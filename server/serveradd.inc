<?php

// set vars
$msg = '';

// check if form is submitted
if (!empty($_POST)){
	if (isset($_POST['ctrl_svr']) && isset($_POST['ctrl_addr']) && isset($_POST['ctrl_user']) && isset($_POST['ctrl_pass'])) {
		if (strstr($_POST['ctrl_user'], '@')) {
			$msg .= "<img src=gfx/ico_warn.gif> E-mail address in 'CTRL account' is not allowed. (Check also ctrlaccounts.tab: in case, fix it)<br>";
		} else {
			if (trim($_POST['ctrl_svr']) != "" && trim($_POST['ctrl_addr']) != "" && trim($_POST['ctrl_user']) != "" && trim($_POST['ctrl_pass']) !='') {
				$ctrl_pass = xmcrypt($_POST['ctrl_pass']);
				$servers = serverlist();
				if (!isset($_POST['use_ssh'])) $_POST['use_ssh'] = 0;
				$servers[$_POST['ctrl_svr']] = array($_POST['ctrl_addr'], $_POST['ctrl_prt'], $_POST['ctrl_user'], $ctrl_pass, $_POST['use_ssh']);
				if (serverwrite($servers)) {
					$msg .= "<img src=gfx/ico_check.gif>&nbsp;Server <b>$_POST[ctrl_svr]</b> was added successfully.<br>";
					// if was the first server that is added notice user about the Login
					if (count(serverlist($cfg_servers)) == 1){
						$msg .= "<img src=gfx/ico_info.gif>&nbsp;Providing that the setup was correct, it's possible now to access to Xmail server by selecting Login.<br><hr>";
					}
				} else {
					$msg .= '<img src=gfx/ico_warn.gif> Error adding server: ' . $mail_server->xm_err_msg ."<br>";
				}
			} else {
				$msg .= "<img src=gfx/ico_warn.gif> Please fill all the fields.<hr>";
			}
		}
	}
}
echo $msg ."<br>";

?>

<form name='frmNewServer' action='' method='post'>
    <input type='hidden' name='action' value='serveradd'>
    <table border='0' class='tbl'>
        <tr>
            <td><b>Server hostname:</b></td>
            <td><input type='text' name='ctrl_svr' size='30' style='width: 400px;'></td>
        </tr>
        <tr>
            <td><b>Server ip address:</b></td>
            <td><input type='text' name='ctrl_addr' size='30' style='width: 400px;'></td>
        </tr>
        <tr>
            <td><b>Server port:</b></td>
            <td><input type='text' name='ctrl_prt' size='30' style='width: 400px;' value='6017'></td>
        </tr>
        <tr>
            <td><b>CTRL account:</b></td>
            <td><input type='text' name='ctrl_user' size='30' style='width: 400px;'></td>
        </tr>
        <tr>
            <td><b>CTRL password:</b></td>
            <td><input type='password' name='ctrl_pass' size='30' style='width: 400px;'></td>
        </tr>
        <tr>
            <td colspan="2" align="right"><input type='submit' value=' submit '></td>
        </tr>
    </table>
</form>
<script type="text/javascript"><!--
parent.menu.location.reload();
//-->
</script>
<hr>
<?php
if (count(serverlist($cfg_servers)) == 1){
	echo "<img src=gfx/ico_notice.gif>&nbsp;If the entered information was wrong it will not be possible to login or return to this page: delete the <b>'server.php'<b> file to resume from this";}
?>
<br><br>
