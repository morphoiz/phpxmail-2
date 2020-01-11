<?php
// module functions ---------------------------------------------------------------------
//Create an HTML SELECT object with a selection of all users of a domain
function UserSelection($objName, $opts, $usDomain, $usUser){	//select user from a list

echo '<select name="' .$objName .'" ' .$opts .'style="width: 204pt;">';
$mail_server = $_SESSION['mail_server'];
foreach($mail_server->userlist($usDomain) as $user){
	if (strtoupper($user[3]) == 'U'){
		echo '<option';
		if ($user[1] == $usUser) { echo ' selected';}
		echo ' value="' .$user[1] .'">'.$user[1] .'@' .$usDomain .'</option>';
	}
}
echo '</select>';
}
// end of module functions --------------------------------------------------------------

$IPobj = new IP_object;

$msg = '';
$vars = '';
$ErrorDetected = false;
$needFineTune = false;
$file = "domains\\$domain\\$username\\pop3.ipmap.tab";

//get the Ip-Map data from file
$vars = $mail_server->cfgfileget($file);
if (is_array($vars)){
	$fnum = (count($vars));
}else{
	$fnum=0;
}

// check if form is submitted
if (!empty($_POST)){
	$data = array();
	// check if submitted from SAVE pushbutton or from PLUS (add) icons
	if (isset($_POST['cmdSave']) || isset($_POST['cmdAdd_x'])){
		//add existing Ip-Map setting
		for ($n = 1; $n <= $fnum; $n++) {
			$IPadd = $_POST[$n . "_IPadd"];
			if (!$IPobj->trueIP($IPadd, false)) {
				$ErrorDetected=true;
				$msg .= "<img src=gfx/ico_warn.gif> Invalid IP address: <B>" .$IPadd ."</B> in line " .$n ."<br>";
			}
			$IPmask = $_POST[$n . "_IPmask"];
			if (!$IPobj->trueIP($IPmask, true)) {
				$ErrorDetected=true;
				$msg .= "<img src=gfx/ico_warn.gif> Invalid IP mask: <B>" .$IPmask ."</B> in line " .$n ."<br>";
			}
			$Perm = strtoupper($_POST[$n . "_Perm"]);
			if ($Perm == "ALLOW" || $Perm == "DENY"){
			} else {
				$ErrorDetected=true;
				$msg .= "<img src=gfx/ico_warn.gif> Invalid permission value: '<B>" .$Perm ."'</B> in line " .$n ."<br>";
			}
			$Prior = $_POST[$n . "_Prior"];
			if (is_numeric($Prior) && $Prior > 0) {
				//It's all OK, so add the line to the writing array...
			} else {
				$ErrorDetected=true;
				$msg .= "<img src=gfx/ico_warn.gif> Invalid priority value: <B>" .$Prior ."</B> in line " .$n ."<br>";
			}
			array_push($data, array("\"$IPadd\"","\"$IPmask\"","\"$Perm\"","\"$Prior\""));
		}
	}
	// check if submitted from PLUS (add) icons
	if (isset($_POST['cmdAdd_x'])){
		//	if (isset($_POST['xtr_IPadd'])){
		if ($_POST['xtr_IPadd'] .$_POST['xtr_IPmask'] .$_POST['xtr_Prior'] != "" ){
			// check if all fiels are compiled
			if ($_POST['xtr_IPadd'] != "" && $_POST['xtr_IPmask'] != "" && $_POST['xtr_Perm'] != "" && $_POST['xtr_Prior'] != "" ){
				//get fields and add new Ip-Map
				$xtr_IPadd = $_POST["xtr_IPadd"];
				if (!$IPobj->trueIP($xtr_IPadd, false)) {
					$ErrorDetected=true;
					$msg .= "<img src=gfx/ico_warn.gif> Invalid IP address: <B>" .$xtr_IPadd ."</B><br>";
				}
				$xtr_IPmask = $_POST["xtr_IPmask"];
				if (!$IPobj->trueIP($xtr_IPmask, true)) {
					$ErrorDetected=true;
					$msg .= "<img src=gfx/ico_warn.gif> Invalid IP mask: <B>" .$xtr_IPmask ."</B><br>";
				}
				$xtr_Perm = strtoupper($_POST["xtr_Perm"]);
				if ($xtr_Perm == "ALLOW" || $xtr_Perm == "DENY"){
					$xtr_Prior = $_POST["xtr_Prior"];
					if (is_numeric($xtr_Prior) && $xtr_Prior > 0) {
						//It's all OK, so add the line to the writing array...
						array_push($data, array("\"$xtr_IPadd\"","\"$xtr_IPmask\"","\"$xtr_Perm\"","\"$xtr_Prior\""));
					} else {
						$ErrorDetected=true;
						$msg .= "<img src=gfx/ico_warn.gif> Invalid priority value: <B>" .$xtr_Prior ."</B><br>";
					}
				} else {
					$ErrorDetected=true;
					$msg .= "<img src=gfx/ico_warn.gif> Invalid permission value: '<B>" .$xtr_Perm ."'</B><br>";
				}
			}
			else{
				$ErrorDetected=true;
				$msg .= "<img src=gfx/ico_warn.gif> Please fill all the Ip-Map fields.<br>";
			}
		} else {$ErrorDetected=true;}
	}
	// check if submitted from QUICK-ADD icon
	if (isset($_POST['cmdQuickAdd_x']) || isset($_POST['cmdQuickPreset'])){
		// obtain intranet address where xmail server is running...
		$IP_Add = $mail_server->xm_ip;
//		$IP_Add = '10.133.4.1';  // only for debug...
//		$IP_Add = '218.133.4.114';  // only for debug...
		list ($A1,$A2,$A3) = preg_split('\.', $IP_Add);
		//preset a template for Intranet Class A, B or C
		$NotIntranet=false;
		switch ($A1){
			case '10':  $IP_Add = "10.AA2.AA3.0"; $IP_Msk = "255.B.C.0"; break;
			case '172': $IP_Add = "172.AA2.AA3.0"; $IP_Msk = "255.240.C.0"; break;
			case '192': $IP_Add = "192.168.AA3.0"; $IP_Msk = "255.255.C.0"; break;
			default: $IP_Msk = "255.255.255.255";
			$NotIntranet = true;
		}
		$needFineTune = false; //reset notice of fine tuning
		switch ($_POST['PresetType']){
			case 'Everywhere':
			array_push($data, array("\"0.0.0.0\"","\"0.0.0.0\"","\"ALLOW\"","\"1\""));
			break;
			case 'INTERNET only':
			array_push($data, array("\"0.0.0.0\"","\"0.0.0.0\"","\"ALLOW\"","\"1\""));
			array_push($data, array("\"10.0.0.0\"","\"255.0.0.0\"","\"DENY\"","\"2\""));
			array_push($data, array("\"172.16.0.0\"","\"255.240.0.0\"","\"DENY\"","\"3\""));
			array_push($data, array("\"192.168.0.0\"","\"255.255.0.0\"","\"DENY\"","\"4\""));
			array_push($data, array("\"127.0.0.1\"","\"255.255.255.255\"","\"DENY\"","\"5\""));
			break;
			case 'Intranet (class A)';
			$IP_Add = str_replace(array('AA2','AA3'), '0', $IP_Add);
			$IP_Msk = str_replace(array('C','B'), '0', $IP_Msk);
			array_push($data, array("\"0.0.0.0\"","\"0.0.0.0\"","\"DENY\"","\"1\""));
			array_push($data, array("\"$IP_Add\"","\"$IP_Msk\"","\"ALLOW\"","\"2\""));
			$needFineTune = true;
			if ($NotIntranet) $msg .= "<img src=gfx/ico_warn.gif> Intranet's IP ranges not detected: substituted with server IP address '" . $IP_Add ."'<br>";
			break;
			case 'Intranet (class B)';
			$IP_Add = str_replace('AA2', $A2, $IP_Add);
			$IP_Add = str_replace('AA3', '0', $IP_Add);
			$IP_Msk = str_replace('C', '0', $IP_Msk);
			$IP_Msk = str_replace(array('B','240'), '255', $IP_Msk);
			array_push($data, array("\"0.0.0.0\"","\"0.0.0.0\"","\"DENY\"","\"1\""));
			array_push($data, array("\"$IP_Add\"","\"$IP_Msk\"","\"ALLOW\"","\"2\""));
			$needFineTune = true;
			if ($NotIntranet) $msg .= "<img src=gfx/ico_warn.gif> Intranet's IP ranges not detected: substituted with server IP address '" . $IP_Add ."'<br>";
			break;
			case 'Intranet (class C)';
			$IP_Add = str_replace('AA2', $A2, $IP_Add);
			$IP_Add = str_replace('AA3', $A3, $IP_Add);
			$IP_Msk = str_replace(array('C','B','240'), '255', $IP_Msk);
			array_push($data, array("\"0.0.0.0\"","\"0.0.0.0\"","\"DENY\"","\"1\""));
			array_push($data, array("\"$IP_Add\"","\"$IP_Msk\"","\"ALLOW\"","\"2\""));
			$needFineTune = true;
			if ($NotIntranet) $msg .= "<img src=gfx/ico_warn.gif> Intranet's IP ranges not detected: substituted with server IP address '" . $IP_Add ."'<br>";
			break;
			case 'Server address only';
			array_push($data, array("\"0.0.0.0\"","\"0.0.0.0\"","\"DENY\"","\"1\""));
			array_push($data, array("\"$mail_server->xm_ip\"","\"255.255.255.255\"","\"ALLOW\"","\"2\""));
			break;
			case 'Local machine only';
			array_push($data, array("\"0.0.0.0\"","\"0.0.0.0\"","\"DENY\"","\"1\""));
			array_push($data, array("\"127.0.0.1\"","\"255.255.255.255\"","\"ALLOW\"","\"2\""));
			break;
		}
	}
	if (isset($_POST['cmdOtherUser_x'])){
	}


	// Write modifyied and/or added lines
	if (!$ErrorDetected){
		// Sort $data with PRIORITY ascending, ADDRESS ascending
		foreach ($data as $key => $row) {  // Obtain a list of columns
		$rowPrior[$key]  = $row[3];
		$rowAddr[$key] = $row[0];
		}
		array_multisort($rowPrior, SORT_ASC, $rowAddr, SORT_ASC, $data); // $data as the last parameter, to sort by the common key

		if ($mail_server->cfgfileset($file, $data)) {
			$msg .= "<img src=gfx/ico_check.gif> Ip-Maps in 'pop3.ipmap.tab' were saved successfully";
		} else {
			$msg .= "<img src=gfx/ico_warn.gif> Error saving Ip-Maps in 'pop3.ipmap.tab': " . $mail_server->xm_err_msg;
		}
	}
}
else {
	// check if X (delete) icon was used
	if (isset($_GET['do']) && isset($_GET['line'])){
		//delete Ip-Maps
		if ($_GET['do'] == "delete"){
			$line = $_GET['line'];
			$i = 1;
			$data = array();
			foreach ($vars as $var) {
				if ($i != $line){
					//add commas
					unset($d);
					foreach ($var as $v){
						$d[] = "\"$v\"";
					}
					$data[] = $d;
					//added
				}
				$i++;
			}
			//		if (is_array($data)){
			$write = $mail_server->cfgfileset($file, $data); //if no-data, optional file will be deleted
			//		}else{
			//			$write = $mail_server->cfgfileclear($file, $data);
			//		}
			if ($write) {
				$msg .= "<img src=gfx/ico_check.gif> Ip-Map line $line was deleted successfully";
			} else {
				$msg .= "<img src=gfx/ico_warn.gif> Error deleting Ip-Map line $line in 'pop3.ipmap.tab': " . $mail_server->xm_err_msg;
			}
		}
	}
}

echo $msg;

// Body of frame page
?>
<form name="frmIpMap" action="" method="post">
<table width="600">
	<tr><td width="100%">
	<fieldset class="caption" style="width=100%"><legend><b>POP3 Map settings:</b>&nbsp;</legend>
	    <input type='hidden' name='action' value='pop3ipmap'>
	    <input type='hidden' name='domain' value='<?php echo $domain; ?>'>
	    <input type='hidden' name='username' value='<?php echo $username; ?>'>
	
		<table width="100%">
	    <tr>
	        <td width="10">&nbsp;</td>
	        <td width="100"><b>IP address</b></td>
	        <td width="100"><b>IP mask</b></td>
	        <td width="100"><b>Permission</b></td>
	        <td width="100"><b>Precedence</b></td>
	    </tr>
	    <tr>
	    	<td><input class="transparent" type="image" name="cmdAdd" alt="Add" value="Add" src=gfx/ico_create.gif style="width=16px"></td>
	        <td><input type='text' name='xtr_IPadd'></td>
	        <td><input type='text' name='xtr_IPmask'></td>
	        <td><select name="xtr_Perm"><option selected>ALLOW</option><option>DENY</option></select></td>
	        <td><input type='text' name='xtr_Prior'></td>
	    </tr>
	    <tr><td colspan="5">&nbsp;</td></tr>
	    <?php $i=0; 
	    // Read back for display all changes
	    $vars = $mail_server->cfgfileget($file);
	    // show all stored lines
	    if (is_array($vars)){
	    	echo '<tr>';
	    	foreach ($vars as $var) { $i++; if(is_array($var)){ ?>
	            <tr>
	               <td><a href=main.php?action=pop3ipmap&domain=<?php echo $domain; ?>&username=<?php echo $username; ?>&do=delete&line=<?php echo $i; ?>><img src=gfx/ico_delete.gif border=0></a></td>
	               <td><input type='text' name='<?php echo $i; ?>_IPadd' value='<?php echo($var[0]); ?>'></td>
	               <td><input type='text' name='<?php echo $i; ?>_IPmask' value='<?php echo($var[1]); ?>'></td>
				   <td><select name='<?php echo $i; ?>_Perm'>
				       <?PHP 
				       if ($var[2]=="ALLOW") {
				       	echo '<option selected>ALLOW</option><option>DENY</option></select>';
				       } else {
				       	if ($var[2]=="DENY") {
				       		echo '<option>ALLOW</option><option selected>DENY</option></select>';
				       	} else {
				       		echo '<option>ALLOW</option><option>DENY</option><option selected>' .$var[2] .'</option></select>';
				       	}
				       } ?>
				       </td>
				    <td><input type='text' name='<?php echo $i; ?>_Prior' value='<?php echo($var[3]); ?>'></td>
				 </tr>
	    	<?php }} 
	    	echo '<tr>';
	    	if ($i > 1) {
	    		echo '<td><img src=gfx/ico_info.gif align=absmiddle title=info></td>';
	    		echo '<td colspan="3">Higher precedences win over lower ones.</td>';
	    		echo '<td align="right">';
	    	} else {
	    		echo '<td align="right" colspan="5">';
	    	}
	    	echo '<p>&nbsp;<input type="submit" name="cmdSave" value="Save values" style="width=100"></td>';
	    	echo '</tr>';
	    	if ($needFineTune) {
	    		echo '<tr><td colspan="5">&nbsp;</td></tr>';
	    		echo '<tr>';
	    		echo '<td valign="top"><img src=gfx/ico_notice.gif align=absmiddle title=notice></td>';
	    		echo '<td colspan="4" valign="top">After using "Intranet (class X)" in "Quick setup" you may need to <br>fine tune the new settings, so they will best fit your local network. </td>';
	    		echo '</tr>';
	    	}
	    } ?>
	</fieldset>
</table>
</td></tr>
	<tr><td colspan="5">&nbsp;</td></tr>
	<tr><td>
	<fieldset class="caption" style="width=100%"><legend><b>Quick setup:</b>&nbsp;</legend>
    <table width="100%">
		<tr>
            <td>
            	<input class="transparent" type="image" name="cmdQuickAdd" alt="Preset" value="Preset" src=gfx/ico_filter_on.gif style="width=16px"></td>
			<td colspan="2">Allow POP3 access from this network: </td>
            <td align="right">
            	<select name="PresetType">
				<option selected>Everywhere</option>
				<option>INTERNET only</option>
				<option>Intranet (class A)</option>
				<option>Intranet (class B)</option>
				<option>Intranet (class C)</option>
				<option>Server address only</option>
				<option>Local machine only</option>
				</select>
			</td>
			<td align="right">
    			<input align="right" type="submit" name="cmdQuickPreset" value=" Preset " style="width=100">
    		</td>
        </tr>
        <tr>
            <td>&nbsp;</td>
            <td colspan="4">(Warning: all current settings will be overwritten!)</td>
        </tr>
    	<tr>
	    	<td width="16px">&nbsp;</td>
	        <td width="106"><input class="transparent"></td>
	        <td width="106"><input class="transparent"></td>
	        <td width="106"><input class="transparent"></td>
	        <td width="106"><input class="transparent"></td>
		</tr>
        <tr>
            <td >
				<a><img src=gfx/ico_see.gif border=0 onclick="goto_user()"></a></td>
			<td colspan="2">Show/modify the above settings for: </td>
        	<td colspan="2"><?php UserSelection('transv_user', 'onchange="goto_user()"', $domain, $username); ?></td>
        </tr>
	</table>
	</fieldset>
</td></tr>
</table>
</form>

<script type="text/javascript"><!--
document.frmIpMap.xtr_IPadd.focus();

function goto_user()
{
	var box = document.forms['frmIpMap'].transv_user;
	destination = "main.php?action=pop3ipmap&domain=<?php echo $domain; ?>&username=" + box.options[box.selectedIndex].value;
	//	document.write (destination);
	parent.main.location.href = destination;
}
//-->
</script>
<hr>
<br><br>