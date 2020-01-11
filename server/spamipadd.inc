<?php

$msg = '';
if (isset($_GET['adr'])) {$S_addr = $_GET['adr']; $readonly = true; } else {$S_addr =''; $readonly = false;}
if (isset($_GET['msk'])) $S_mask = $_GET['msk']; else $S_mask ='';
if (isset($_GET['act'])) $S_action = $_GET['act']; else $S_action ='';
if (isset($_GET['dly'])) $S_delay = $_GET['dly']; else $S_delay ='';

// check if form is submitted
if (!empty($_POST)) {
	if (isset($_POST['readonly'])) $readonly = $_POST['readonly']; // was EDIT instead of ADD: IP field will not be editable
	if (!empty($_POST['spamaddr'])){
		$IP = new IP_object();
		// check IP validity
		if ($IP->trueIP($_POST['spamaddr'], false)){
			$S_addr = $IP->IP;
			if (!$IP->ValidRange) {
				// was not in the form aaa.bbb.ccc.ddd/mm
				// so also Mask field must be checked
				if (!$IP->trueIP($_POST['spammask'], true)){
					$msg .= '<img src=gfx/ico_warn.gif> <b>' .$_POST['spammask'] .'</b> is not a valid Mask range<br>'; //.': ' .$mail_server->xm_err_msg;
				}
			}
			$S_mask = $IP->Mask;
			$S_action = $_POST['spamaction'];
			// set delay accordlyng with action value
			if ($S_action == 2) {
				$S_delay = $_POST['spamdelay'];
				if (empty($S_delay) || ($S_delay == 0)) $S_delay = 1;
			} else {
				$S_delay = '';
			}
			if (SpamIpListAdd(array($S_addr, $S_mask, $S_action, $S_delay))){
				$readonly = false;
				$msg .= "<img src=gfx/ico_check.gif> Address <b>" .$_POST['spamaddr'] ."</b> successfully stored in the local spam list.<br>";
			} else {
				$msg .= '<img src=gfx/ico_warn.gif> Error storing <b>' .$_POST['spamaddr'] .'</b> in the local spam list.<br>'; //.': ' .$mail_server->xm_err_msg;
			}
		} else {
			$msg .= '<img src=gfx/ico_warn.gif> <b>' .$_POST['spamaddr'] .'</b> is not a valid IP address<br>'; //.': ' .$mail_server->xm_err_msg;
		}
	} else {
		$msg .= '<img src=gfx/ico_warn.gif> At least a valid IP address must be supplied<br>'; //.': ' .$mail_server->xm_err_msg;
	}
}

echo $msg;
?>

<form name='frmNewIpSpam' action='main.php' method='post'>
    <input type='hidden' name='action' value='spamipadd'>
    <input type='hidden' name='readonly' value='<?php echo $readonly;?>'>
    <table border='0' class='tbl'>
        <tr>
            <td><b>Spam address:<b></td>
            <td><input type='text' name='spamaddr' <?php if ($readonly) echo "readonly='yes'";?> value='<?php echo $S_addr;?>' size='30' style='width: 200;'></td>
        </tr>
        <tr>
            <td><b>Mask range:<b></td>
            <td><input type='text' name='spammask' value='<?php echo $S_mask;?>' size='30' style='width: 200;'></td>
        </tr>
        <tr>
            <td><b>SMTP action:<b></td>
            <td>
            	<select name='spamaction' style='width: 200;'>
            	<option value="0" <?php if ($S_action == 0) echo 'selected';?>>always blocked</option>
            	<option value="1" <?php if ($S_action == 1) echo 'selected';?>>authenticated only</option>
            	<option value="2" <?php if ($S_action == 2) echo 'selected';?>>delayed commands</option>
            	</select>
            </td>
        </tr>
        <tr>
            <td><b>delay:<b></td>
            <td><input type='text' name='spamdelay' value='<?php echo $S_delay;?>' size='4' style='width: 70;'></td>
        </tr>
        <tr>
            <td colspan="2" align="right"><input type='submit' value=' submit '></td>
        </tr>
    </table>
</form>
<br>
<hr>
<?php 
if (!$readonly) { ?>
	<img src='gfx/ico_info.gif' alt='Info' height='16' width='16' border='0' valign="top"> Mask may be introduced also appending <b>/nn</b> at the end of IP address (nn = bits of the mask)</p>
<?php } ?>

<!-- Set focus on 1st field... -->
<script type="text/javascript"><!--
document.frmNewIpSpam.spamaddr.focus();
//-->
</script>