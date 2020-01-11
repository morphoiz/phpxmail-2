<?php

$servers = serverlist();
$msg = '';

// check if form is submitted
if (isset($_POST['server'])) {
	$mail_server = new XMail();
	$s = $_POST['server'];
	$mail_server->xm_server = $s;
	$mail_server->xm_ip = $servers["$s"][0];
	$mail_server->xm_port = $servers["$s"][1];
	$mail_server->xm_ctrl_u = $servers["$s"][2];
	$mail_server->xm_ctrl_p = xmdecrypt($servers["$s"][3]);
	if (isset($_POST['submit'])) {
		if ($_POST['username'] && $_POST['password'] && $_POST['domain']){
			//check for mailboxes limits
			$Settings = getDomainSettings($domain);
			$MaxMB = $Settings['MaxMailboxes'];
			unset ($Settings);
			$i=0;
			$users = $mail_server->userlist($domain);
			foreach ($users as $user){
				if (strtoupper($user[3]) == 'U') $i++;
			}
			if (($i < $MaxMB) || ($MaxMB == 0)) {
				if ($mail_server->useradd($_POST['domain'], $_POST['username'], $_POST['password'], 'U')) {
					echo "<p>&nbsp;</p>";
					echo "<img src=gfx/ico_check.gif> New user <b>$_POST[username]@$domain</b> created successfully.<br><br>";
					echo "<img src=gfx/ico_info.gif> You can now proceed to log in to the selected server with your email address as your username";
					die;
				} else {
					$msg = '<img src=gfx/ico_warn.gif> Error adding user: ' . $mail_server->xm_err_msg;
				}
			} else {
				$msg = '<img src=gfx/ico_warn.gif> Max number of users has been reached: the registering is refused';
			}
		} else {
			$msg = "<img src=gfx/ico_warn.gif> Please fill in the fields of the form.";
		}
	}
}

?>

<form name='frmServer' action='main.php' method='post'>
    <input type='hidden' name='action' value='register'>
    <?php echo "$msg"; ?>
    <table border='0' class='tbl'>
		<tr>
		    <td><b>Server name :&nbsp;&nbsp;<b></td>
		    <td>
	            <select name='server' onchange='javascript:form.submit();' style='width: 400px;'>
					<option>Select a server</option>
					<?php
					foreach($servers as $svr=>$svr_info) {
						if (isset($_POST['server'])){
							$selected = ($svr == $_POST['server']) ? 'selected' : '';
						} else {
							$selected = "";
						}
						echo "<option value='" .$svr ."'" .$selected .">" .$svr ."</option>";
					}
					?>
	            </select>
		    </td>
		</tr>
    </table>
</form>

<form name='frmNewUser' action='main.php' method='post'>
	<input type='hidden' name='action' value='register'>
	<input type='hidden' name='server' value="<?php if (isset($_POST['server'])){echo $_POST['server'];} ?>">
	<table border='0' class='tbl'>
	    <tr>
	        <td><b>User name *:<b></td>
	        <td><input type='text' name='username' size='30' style='width: 200px;'> @
                <select name='domain' style='width: 180px;'>
                    <?php if (isset($_POST['server'])){  foreach($mail_server->domainlist() as $domain) { ?>
                        <option value=<?php echo "'$domain'"; ?>><?php echo $domain; ?></option>
                    <?php }} ?>
                </select>
	        </td>
	    </tr>
	    <tr>
	        <td><b>User password:</b></td>
	        <td><input type='password' name='password' size='30' style='width: 400px;'>
	    </tr>
	    <tr>
	        <td colspan="2" align="right"><input type='submit' name='submit' value=' submit '>
	    </tr>
	</table>
</form>

<hr>
<img src=gfx/ico_info.gif align=absmiddle title=info> 
* to register a mail account select a user name and a server on which the email account should be created
<br><br>