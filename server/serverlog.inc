<?php
function RecordsCount($f){
	if (is_array($f)){
		if (isset($f[0])){
			if (is_array($f[0])){
				return count($f);
			} else {
				if (trim($f[0]) != "") {
					return count($f);
				} else {
					return count($f)-1;
				}
			}
		}
	}
	return 0;
}

$msg = '';

// check if form is submitted
if (!empty($_GET)) {
	$OnlyErrors=false;
	if (isset($_GET['erronly'])) {
		if ($_GET['erronly']) $OnlyErrors = true;
	}
	if ($_GET['do'] == "empty") {
		$FileToempty = $_GET['ltype'] .'-' .$_GET['log'];
		if ($mail_server->cfgfileclear('logs/'.$FileToempty)) {
			$msg = "<img src=gfx/ico_check.gif> Log file <b>'".$FileToempty."'</b> emptied successfully.";
		} else {
			$msg = '<img src=gfx/ico_warn.gif> Error clearing log: ' . $mail_server->xm_err_msg;
		}
		echo $msg ."<hr>";
		$MustShown = true;
	}
	if ($_GET['do'] == "spam") {
		if (isset($_GET['spamaddr'])){
			if (SpamListAdd($_GET['spamaddr'])){
				$msg = "<img src=gfx/ico_check.gif> Address <b>" .$_GET['spamaddr'] ."</b> successfully added to local spam list.";
			} else {
				$msg = '<img src=gfx/ico_warn.gif> Error adding <b>' .$_GET['spamaddr'] .'</b> to local spam list.'; //.': ' .$mail_server->xm_err_msg;
			}
		}
		if (isset($_GET['spamip'])){
			if (SpamIpListAdd(array($_GET['spamip'], '255.255.255.255', '0', ''))){
				$msg = "<img src=gfx/ico_check.gif> IP Address <b>" .$_GET['spamip'] ."</b> successfully added to local IP spam list.";
			} else {
				$msg = '<img src=gfx/ico_warn.gif> Error adding <b>' .$_GET['spamip'] .'</b> to local IP spam list.'; //.': ' .$mail_server->xm_err_msg;
			}
		}
		echo $msg ."<hr>";
		$MustShown = true;
	}	
	if (isset($_GET['log'])){
		$yy = substr($_GET['log'], 0, 4);	//selected year
		$mm = substr($_GET['log'], 4, 2);	//selected month
		$ff = substr($_GET['log'], 6, 6);	//selected file

		// Read the name of all the files of this log type
		$Files = $mail_server->filelist('logs', $_GET['ltype'] .'-*');
		if (count($Files) == 0) {
			echo '<img src=gfx/ico_warn.gif>&nbsp;Does not exists any log file!<hr>';
			die;
		} else {
			$LogFiles = array();
			$offset = strlen($_GET['ltype']) +1;
			//Reverse the order, so today will be at the top of the drop down lists
			rsort($Files);
			//Create an array whith Year, Month & File as indexes, size as value.
			foreach ($Files as $sFile) {
				$LogFiles[substr($sFile[0], $offset, 4)][substr($sFile[0], $offset +4, 2)][substr($sFile[0], $offset +6, 20)] = "(" .humansize($sFile[1]) .")";
			}
		}
		unset ($Files);
		?>
		<form name="frmDateSelection" id="frmDateSelection" action="">
			<a href="main.php?action=serverlog&ltype=<?php echo $_GET['ltype']; ?>&log=<?php echo $_GET['log']; ?>&do=empty"><img src="gfx/ico_delete.gif" alt="Empty records of this file" border="0"></a>
			<input type="hidden" name="ltype" value="<?php echo $_GET['ltype'] ?>">
			<input type="hidden" name="log" value="<?php echo $_GET['log'] ?>">
			<select name="s_year" id="s_year" style="width: 50pt;" onchange="YearChange()">
			</select>&nbsp;
			<select name="s_month" id="s_month" style="width: 75pt;" onchange="MonthChange()">
			</select>&nbsp;
			<select name="s_file" id="s_file" style="width: 150pt;"  onchange="FileChange()">
			</select>&nbsp
			<input name="go" id="go" type="button" value=" Go " disabled onclick="GotoFile()">
		</form>
	
		<script  type="text/javascript">
		//Init drop down menues
		var LogFiles = [];
		var MonthName = [];
		var sel_yy = "<?php echo $yy ?>";
		var sel_mm = "<?php echo $mm ?>";
		var sel_ff = "<?php echo $ff ?>";

		<?php
		// Preset an array with names of all the months
		for ($i=1; $i<13; $i++){
			?>
			MonthName["<?php echo sprintf("%02d",$i) ?>"]=	"<?php echo date('F', strtotime("2005" .sprintf("%02d",$i) ."01"))?>";
			<?php
		}
		foreach ($LogFiles as $key_year=>$yyy) {
			?>
			LogFiles["<?php echo $key_year ?>"]=[];
			<?php
			foreach ($yyy as $key_month=>$mmm) {
				unset($key_array2);
				foreach ($mmm as $key_day=>$ddd) {
					$key_array2[] = $key_day;
					$key_array3[] = $ddd;
				}
				?>
				LogFiles["<?php echo $key_year ?>"]["<?php echo $key_month ?>"]=[];
				LogFiles["<?php echo $key_year ?>"]["<?php echo $key_month ?>"]["name"]=[];
				LogFiles["<?php echo $key_year ?>"]["<?php echo $key_month ?>"]['size']=[];
				LogFiles["<?php echo $key_year ?>"]["<?php echo $key_month ?>"]["name"]=["<?php echo implode('", "', $key_array2)?>"];
				LogFiles["<?php echo $key_year ?>"]["<?php echo $key_month ?>"]['size']=["<?php echo implode('", "', $key_array3)?>"];
				<?php
			}
		}
		unset($LogFiles);
		?>
		List1 = document.getElementById("s_year");
		List2 = document.getElementById("s_month");
		List3 = document.getElementById("s_file");
		Button_Go = document.getElementById("go");
		jsSetYear();

		// aux - init functions
		function jsSetYear(){
			List1.options.length = 0;
			$i=0; $Sel = 0;
			for (year in LogFiles){
				List1.options[$i] = new Option(year, year);
				if (year == sel_yy) $Sel = $i;
				$i++;
			}
			List1.options[$Sel].selected=true;
			jsSetMonth(List1.options[List1.options.selectedIndex].value);
		}

		function jsSetMonth(y){
			List2.options.length = 0;
			$i=0; $Sel = 0;
			for (month in LogFiles[y]){
				List2.options[$i] = new Option(MonthName[month], month);
				if (month == sel_mm) $Sel = $i;
				$i++;
			}
			List2.options[$Sel].selected=true;
			jsSetFile(y,List2.options[List2.options.selectedIndex].value);
		}

		function jsSetFile(y,m){
			List3.options.length = 0;
			$i=0; $Sel = 0;
			for (file in LogFiles[y][m]["name"]){
				$ftemp = LogFiles[y][m]["name"][file];
				switch ($ftemp.substr(1,1)){
					case '1':
					$ftemp = $ftemp.substr(0,2) +'st, ' + $ftemp.substr(2,2) +':' + $ftemp.substr(4,2);
					break;
					case '2':
					$ftemp = $ftemp.substr(0,2) +'nd, ' + $ftemp.substr(2,2) +':' + $ftemp.substr(4,2);
					break;
					case '3':
					$ftemp = $ftemp.substr(0,2) +'rd, ' + $ftemp.substr(2,2) +':' + $ftemp.substr(4,2);
					break;
					default:
					$ftemp = $ftemp.substr(0,2) +'th, ' + $ftemp.substr(2,2) +':' + $ftemp.substr(4,2);
					break;
				}
				$ftemp += " " +LogFiles[y][m]['size'][file];
				List3.options[$i] = new Option($ftemp, LogFiles[y][m]["name"][file]);
				if (LogFiles[y][m]["name"][file] == sel_ff) $Sel = $i;
				$i++;
			}
			List3.options[$Sel].selected=true;
		}

		// -- Client actions --
		function YearChange(){
			sel_yy = List1.options[List1.options.selectedIndex].value;
			jsSetYear();
			Button_Go.disabled = false;
		}

		function MonthChange(){
			sel_yy = List1.options[List1.options.selectedIndex].value;
			sel_mm = List2.options[List2.options.selectedIndex].value;
			sel_ff ='0';
			jsSetYear();
			Button_Go.disabled = false;
		}

		function FileChange(){
			Button_Go.disabled = false;
			GotoFile();
		}

		function ErrOnlyChange(){
			Button_Go.disabled = false;
			GotoFile();
		}

		function GotoFile(){
			sel_yy = List1.options[List1.options.selectedIndex].value;
			sel_mm = List2.options[List2.options.selectedIndex].value;
			sel_ff = List3.options[List3.options.selectedIndex].value;
			Err_Only = document.getElementById("erronly");
			destination = "main.php?action=serverlog&ltype=<?php echo $_GET['ltype'] ?>&do=view&log=" + sel_yy + sel_mm + sel_ff;
			if (Err_Only.checked) destination += "&erronly=1";
			parent.main.location.href = destination;
		}

		function Alert($msg){
			alert($msg);
		}
		</script>
		
		<?PHP
		$LogToUse = $_GET['ltype'] .'-' .$_GET['log'];
		if ($_GET['do'] == "view" || $MustShown) {
			$file = $mail_server->cfgfileget('logs/'.$LogToUse);
			if (count($file) >> 10000){die("Log file has more than 10000 entries\nRefusing to show it");}
			$MaySelectErrorsOnly = false;
			//preset table headers & behaviors
			switch (strtolower($_GET['ltype'])) {
				//=================================================================================
				case 'ctrl':
				//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
				$TblHeader = array();
				$TblHeader["0"] =array('title'=>'Client IP<br>Address');
				$TblHeader["1"] =array('title'=>'User<br>Name');
				$TblHeader["2"] =array('title'=>'Password');
				$TblHeader["3"] =array('title'=>'Date/Time');
				$TblHeader["4"] =array('title'=>'Req.');
				break;

				//=================================================================================
				case 'fing':
				//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
				$TblHeader = array();
				$TblHeader["0"] =array('title'=>'Host');
				$TblHeader["1"] =array('title'=>'Domain');
				$TblHeader["2"] =array('title'=>'IP Addr.');
				$TblHeader["3"] =array('title'=>'Date/Time');
				$TblHeader["4"] =array('title'=>'Query');
				break;

				//=================================================================================
				case 'filters':
				//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
				$TblHeader = array();
				$TblHeader["0"] =array('title'=>'Sender<br>(Mail From)');
				$TblHeader["1"] =array('title'=>'Recipient<br>(Rcpt To)');
				$TblHeader["2"] =array('title'=>'Local<br>IP add.');
				$TblHeader["3"] =array('title'=>'Remote<br>IP add.');
				$TblHeader["4"] =array('title'=>'Date/Time');
				$TblHeader["5"] =array('title'=>'Filter<br>Type');
				$TblHeader["6"] =array('title'=>'Filter<br>Info');
				$TblHeader["7"] =array('title'=>'Exec<br>Result');
				$TblHeader["8"] =array('title'=>'Exit<br>Code');
				$TblHeader["9"] =array('title'=>'Executable');
				break;

				//=================================================================================
				case 'lmail':
				//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
				$TblHeader = array();
				$TblHeader["0"] =array('title'=>'SMTP<br>Domain');
				$TblHeader["1"] =array('title'=>'Message File');
				$TblHeader["2"] =array('title'=>'Msg.<br>ID');
				$TblHeader["3"] =array('title'=>'Date/Time');
				break;

				//=================================================================================
				case 'pop3':
				//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
				$TblHeader = array();
				$TblHeader["0"] =array('title'=>'Server<br>FQDN');
				$TblHeader["1"] =array('title'=>'Server<br>Domain');
				$TblHeader["2"] =array('title'=>'Client IP<br>Address');
				$TblHeader["3"] =array('title'=>'Date/Time');
				$TblHeader["4"] =array('title'=>'User');
				$TblHeader["5"] =array('title'=>'Password');
				break;

				//=================================================================================
				case 'psync':
				//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
				$TblHeader = array();
				$TblHeader["0"] =array('title'=>'Date/Time');
				$TblHeader["1"] =array('title'=>'Local<br>Domain');
				$TblHeader["2"] =array('title'=>'Local<br>User');
				$TblHeader["3"] =array('title'=>'Remote<br>Domain');
				$TblHeader["4"] =array('title'=>'Remote<br>User');
				$TblHeader["5"] =array('title'=>'Auth.<br>Type');
				$TblHeader["6"] =array('title'=>'Status', 'errcond'=>'=E');
				$TblHeader["7"] =array('title'=>'Msg #');
				$TblHeader["8"] =array('title'=>'Msg<br>Size');
				$TblHeader["9"] =array('title'=>'Msg<br>Err');
				$TblHeader["10"] =array('title'=>'Size<br>Err');
				$MaySelectErrorsOnly = true;
				break;

				//=================================================================================
				case 'smail':
				//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
				$Delivery = array();
				$Delivery['SMTP'] = "Delivery to a remote SMTP server";
				$Delivery['LOCAL'] = "Delivery to a local user";
				$Delivery['FWD'] = "Forward to a different recipent";
				$Delivery['EXTRN'] = "Filtered";
				$Delivery['FILTER'] = "Filtered";
				$Delivery['RLYS'] = "Delivery through a relay SMTP server";
				$Delivery['REDIR'] = "Redirect to a different recipent (can be local)";
				$Delivery['LREDIR'] = "Redirect to a local recipient";

				$TblHeader = array();
				$TblHeader["0"] =array('title'=>'Server<br>Domain');
				$TblHeader["1"] =array('title'=>'Message File');
				$TblHeader["2"] =array('title'=>'Msg.<br>ID');
				$TblHeader["3"] =array('title'=>'Sender<br>(Mail From)');
				$TblHeader["4"] =array('title'=>'Recipient<br>(Rcpt To)');
				$TblHeader["5"] =array('title'=>'Delivery<br>Type', 'legend'=>$Delivery);
				$TblHeader["6"] =array('title'=>'Resolved<br>Rcpt To');
				$TblHeader["7"] =array('title'=>'Date/Time');
				break;

				//=================================================================================
				case 'smtp':
				//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
				$SmtpErr = array();
				$SmtpErr['AUTH=EFAIL:TYPE=PLAIN'] = "Plain Authentication Failed";
				$SmtpErr['AUTH=EFAIL:TYPE=LOGIN'] = "Login Authentication Failed ";
				$SmtpErr['AUTH=EFAIL:TYPE=CRAM-MD5'] = "Cram-Md5 Authentication Failed";
				$SmtpErr['AUTH=EFAIL:TYPE=EXTRN'] = "External Authentication Failed";
				$SmtpErr['DATA=EFILTER'] = "Rejected by SMTP Message Filter";
				$SmtpErr['ETRN=EACCESS'] = "ETRN SMTP Command was disabled (AllowSmtpETRN=0 in Server Config)";
				$SmtpErr['RCPT=ESYNTAX'] = "Syntax error in RCPT TO";
				$SmtpErr['RCPT=EDSBL'] = "Account was disabled (ReceiveEnable=0 in user Mailbox Account)";
				$SmtpErr['RCPT=EFULL'] = "Exceeded storage allocation";
				$SmtpErr['RCPT=EACCESS'] = "Access denied to mailing list";
				$SmtpErr['RCPT=EAVAIL'] = "Mailbox unavailable";
				$SmtpErr['RCPT=ERELAY'] = "Relay was denied";
				$SmtpErr['RCPT=ENBR'] = "Too many recipients (-Sr maxrcpts cmd line swicth, default 100)";
				$SmtpErr['RCPT=OK'] = "Recipient accepted";
				$SmtpErr['RECV=OK'] = "Message sent";
				$SmtpErr['SNDRIP=EIPSPAM'] = "Sender IP was in spammers.tab";
				$SmtpErr['SNDRIP=EIPMAP'] = "Sender IP was positive to maps check (CustMapsList in Server Config)";
				$SmtpErr['SNDRIP=ERDNS'] = "Sender IP reverse DNS Lookup Failed (SMTP-RDNSCheck set in Server Config)";
				$SmtpErr['SMTP=EERRS'] = "Too many errors with the SMTP session (SMTP-MaxErrors set in Server Config)";
				$SmtpErr['SNDR=EEMPTY'] = "Empty return path";
				$SmtpErr['SNDR=ESYNTAX'] = "Syntax error in MAIL FROM";
				$SmtpErr['SNDR=ENODNS'] = "Sender domain had no DNS/MX entries (CheckMailerDomain=1 in Server Config)";
				$SmtpErr['SNDR=ESPAM'] = "Sender Found in spam-address.tab";
				$SmtpErr['SIZE=EBIG'] = "Message exceeds fixed maximum message size (MaxMessageSize in Server Config)";
				$SmtpErr['VRFY=EACCESS'] = "VRFY SMTP Command was disabled (AllowSmtpVRFY=0 in Server Config)";

				$TblHeader = array();
				$TblHeader["0"] =array('title'=>'Server<br>FQDN');
				$TblHeader["1"] =array('title'=>'Server<br>Domain');
				$TblHeader["2"] =array('title'=>'IP<br>Address', 'link'=>'<a href="main.php?action=serverlog&ltype=' .$_GET['ltype'] .'&log=' .$_GET['log'] .'&do=spam&spamip=..." title="Add this address to the local IP spam list">...</a>');
				$TblHeader["3"] =array('title'=>'Date/Time');
				$TblHeader["4"] =array('title'=>'Client Domain');
				$TblHeader["5"] =array('title'=>'Destination<br>Domain');
				$TblHeader["6"] =array('title'=>'Sender<br>(Mail From)', 'link'=>'<a href="main.php?action=serverlog&ltype=' .$_GET['ltype'] .'&log=' .$_GET['log'] .'&do=spam&spamaddr=..." title="Add this address to the local spam list">...</a>');
				$TblHeader["7"] =array('title'=>'Recipient<br>(Rcpt To)');
				$TblHeader["8"] =array('title'=>'Msg.<br>ID');
				$TblHeader["9"] =array('title'=>'Status', 'errcond'=>'=E', 'legend'=>$SmtpErr);
				$TblHeader["10"] =array('title'=>'Authent<br>User');
				$TblHeader["11"] =array('title'=>'Msg<br>Size');
				$TblHeader["12"] =array('title'=>'Client<br>FQDN');
				$MaySelectErrorsOnly = true;
				break;

				//=================================================================================
				default:
				//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
				$TblHeader = array();
				for ($i=0; $i<count($file[0]); $i++){
					$TblHeader["$i"] =array('title'=>'&nbsp');
				}
				break;
			}
			if ($MaySelectErrorsOnly){
				if ($OnlyErrors) {
					echo '&nbsp;<input type="checkbox" name="erronly" id="erronly" value="0" checked onclick="ErrOnlyChange()">&nbsp;<B>Show errors only</B>';
				} else {
					echo '&nbsp;<input type="checkbox" name="erronly" id="erronly" value="0" onclick="ErrOnlyChange()">&nbsp;<B>Show errors only</B>';
				}
			} else {
				// make an hidden field at which javascript may refer to (avoiding object errors )
				echo '<input type="hidden" name="erronly" id="erronly" value="0">';
			}
			// populate headers
			echo '</p><table class="log" style="border-bottom: #dddddd 1px solid">';
			echo '  <tr class="log">';
			$lnk_column = '';
			foreach ($TblHeader as $Key=>$ColHeader) {
				// check for error condition & legend
				if (array_key_exists('errcond', $ColHeader)) $err_column = $Key;
				if (array_key_exists('legend', $ColHeader)) $lgn_column = $Key;
				// check for possible links (may be multiple)
				if (array_key_exists('link', $ColHeader)) $lnk_column .= $Key .'-';
				echo '<th class="log" id="H' .$Key .'">' .$ColHeader['title'] .'</th>';
			}
			echo '  </tr>';
			//popolate rows
			$i=1;
			foreach ($file as $line){
				$attribute = '<font color="blue">';
				$errorInside = false;
				if (isset($err_column)) {
					if (strpos( " " .$line[$err_column], $TblHeader[$err_column]['errcond'])) {
						$attribute = '<font color="red">';
						$errorInside = true;
					}
				}
				if (isset($lgn_column)) {
					$indx = explode(" ", $line[$lgn_column]);
					$InfoText = $TblHeader[$lgn_column]['legend'][$indx[0]];
					echo "\n";
					$InfoText = '<img src="gfx/ico_help.gif" height="16px" width="16px" align="absmiddle" border="0" alt="' .$InfoText .'" onclick="Alert(\'' .$InfoText .'\')">';
				} else {
					$InfoText = "";
				}
				if (is_array($line) && ($errorInside || !$OnlyErrors)){
					echo "\n";
					echo '<tr class="log">';
					$c=0;
					foreach ($line as $var){
						if (isset($lnk_column)) {
							if (strpos($lnk_column, $c .'-') === false){
								$HTMLtext = $var;
							} else {
								$HTMLtext = str_replace('...', $var, $TblHeader[$c]['link']);
							}
						}

						$infoattrib="";
						if (isset($lgn_column)){
							if ($lgn_column == $c) $infoattrib = $InfoText ."&nbsp;";
						}
						echo "\n";
						echo '<td class="log"';
						if ($i % 2 == 0) {
							echo ' bgcolor=#dddddd';
						}
						echo '>' .$infoattrib .$attribute .$HTMLtext .'</td>';
						$c++;
					}
					echo '</tr>';
					$i++;
				}
			}
			echo '</table><br>';
			if (RecordsCount($file) > 0) {
				echo '(<b>' .($i - 1) .'</b> of <b>' .RecordsCount($file) .'</b> log records are shown)';
			} else echo '(No records there were found)';
			echo '<hr>';
			if (isset($lgn_column)) echo '<img src="gfx/ico_info.gif">&nbsp;Clicking inside the table on the icon <img src="gfx/ico_help.gif">, more details will be shown.';
		}
	}
}
unset($TblHeader);
?>