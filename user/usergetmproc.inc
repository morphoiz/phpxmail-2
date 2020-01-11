<?php
// module functions ---------------------------------------------------------------------
//Create an HTML SELECT object with a selection of all users of a domain
function UserSelection($objName, $opts, $usDomain, $usUser){
	//select user from a list
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

$msg = '';
$filename = $username .'/mailproc.tab';

// check if form is submitted
if (isset($_POST['submit'])) {
	$UsrMProc = trim(str_replace('"', '', $_REQUEST['advanced'])); // preserve for show it later
	if ($UsrMProc != '') {
		// create a multiarray that will be passed to Xmail server
		$UsrMProc2 = explode ("\n",  str_replace("\r", '', $UsrMProc));
		foreach ($UsrMProc2 as $key=>$row){
			$UsrMProc2[$key] = str_replace("|", "<tab>", $row);
			$UsrMProc2[$key] = explode ('<tab>', $UsrMProc2[$key] );
			foreach ($UsrMProc2[$key] as $key2=>$row2){
				$UsrMProc2[$key][$key2] = '"' .$row2 .'"';
			}
		}
		//preset a 'saved successfull' message		
		$msg = "<img src=gfx/ico_check.gif> Values for <b>'" .$username ."'</b> saved successfully";
	} else {
		$UsrMProc2 = array();		//empty the array, so the file will be removed
		$msg = "<img src=gfx/ico_check.gif>mailproc.tab for <b>'".$username ."'</b> was deleted successfully";
	}
	if (!$mail_server->cfgfileset('domains/' .$domain .'/' .$filename, $UsrMProc2 )) {
		$msg = '<img src=gfx/ico_warn.gif> Error saving values: ' . $mail_server->xm_err_msg;
	}
}

//re-read all...
//starting from the domain mproc
$DomMProc = '';
$dmproc = $mail_server->cfgfileget('domains/' .$domain .'/mailproc.tab');
if (is_array($dmproc)) {
	// mproc file exists
	foreach ($dmproc as $entry) {
		// line relative to user mail process
		if (is_array($entry) && $entry != ''){
			$DomMProc .= implode("<tab>", str_replace('"', '', $entry)) . "\n";
		} else {
			$DomMProc .= str_replace('"', '', $entry) . "\n";
		}
	}
	$DomMProc = trim($DomMProc);
}

$UsrMProc = '';
// get user mproc
$mproc = $mail_server->cfgfileget('domains/' .$domain .'/' .$filename);
if (is_array($mproc)) {
	// mproc file exists
	foreach ($mproc as $entry) {
		// line relative to user mail process
		if (is_array($entry) && $entry != ''){
			$UsrMProc .= implode("<tab>", str_replace('"', '', $entry)) . "\n";
		} else {
			$UsrMProc .= str_replace('"', '', $entry) . "\n";
		}
	}
	$UsrMProc = trim($UsrMProc);
}

?>

<form name="frmUserMproc" action="" method="post">
    <input type='hidden' name='action' value='usergetmproc'>
    <input type='hidden' name='domain' value='<?php echo $domain; ?>'>
    <input type='hidden' name='username' value='<?php echo $username; ?>'>
    <?php echo $msg; ?>
	<table width="600">
	
		<tr><td width="100%">
			<fieldset class="caption" style="width=100%"><legend><b>Mail processes:</b>&nbsp;</legend>
	        <table class='tbl' width="100%">
	            <?PHP if ($DomMProc != '') { ?>
	            <tr><td colspan="5">
	            		Existing mail processes for the domain:
	            </tr></td>
	            <tr><td colspan="5" align="center">
	                    <textarea name="domainproc" rows="<?php echo count($dmproc); ?>" cols="65" readonly="yes"><?php echo $DomMProc; ?></textarea><br><br>
	            </tr></td>
	            <tr><td colspan="5">
	                    Specific mail processes for <B><?PHP echo $username .'@' .$domain ?></B>:
	            </tr></td>
	            <?PHP } ?>
	            	<tr><td colspan="5" align="center">
	                <textarea name="advanced" rows="5" cols="65"><?php echo $UsrMProc; ?></textarea>
	            </tr></td>
		    	<tr>
			    	<td width="16px">&nbsp;</td>
			        <td width="106"><input class="transparent"></td>
			        <td width="106"><input class="transparent"></td>
			        <td width="106"><input class="transparent"></td>
			        <td width="106"><input class="transparent"></td>
				</tr>
	            <tr>
	            	<td><img src=gfx/ico_info.gif title="info"></td>
	            	<td colspan="3">Use <b>&lt;tab&gt;</b> or vertical bar <B>|</B> for real tabs</td>
	                <td align="right"><input type="submit" name="submit" value=" submit "></td>
	            </tr>
	            <tr>
	            	<td><img src=gfx/ico_info.gif title="info"></td>
	            	<td colspan="3">It is not required to delimit fields with the <b>"</b> characters</td>
	            </tr>
	        </table>
			</fieldset>
	    </td></tr>
	    <tr><td>&nbsp;</td></tr>
	    <tr><td width="100%">
			<fieldset class="caption" style="width=100%"><legend><b>Shortcut:</b>&nbsp;</legend>
		    <table width="100%">
		        <tr>
		            <td >
						<a><img src=gfx/ico_see.gif border=0 onclick="goto_user()"></a></td>
					<td colspan="2">Show/modify the above settings for: </td>
		        	<td colspan="2" align="right"><?php UserSelection('transv_user', 'onchange="goto_user()"', $domain, $username); ?></td>
		        </tr>
		    	<tr>
			    	<td width="16px">&nbsp;</td>
			        <td width="106"><input class="transparent"></td>
			        <td width="106"><input class="transparent"></td>
			        <td width="106"><input class="transparent"></td>
			        <td width="106"><input class="transparent"></td>
				</tr>
			</table>
			</fieldset>
	    </td></tr>
	</table>
</form>
<hr>
<br><br>

<script type="text/javascript"><!--
document.frmUserMproc.advanced.focus();

function goto_user()
{
	var box = document.forms['frmUserMproc'].transv_user;
	destination = "main.php?action=<?php echo $action; ?>&domain=<?php echo $domain; ?>&username=" + box.options[box.selectedIndex].value;
	parent.main.location.href = destination;
}
//-->
</script>
