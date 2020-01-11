<script type="text/javascript"><!--
function LoginRetry(){
	// re-open the login page...
	parent.main.location.href = "main.php?action=login";
}
//-->
</script>
<?php

if (isset($_SESSION['mail_server'])){
	$mail_server = $_SESSION['mail_server'];
}else{  $mail_server = new XMail();}

if (isset($_SESSION['logged_in'])){
	$logged_in  = $_SESSION['logged_in'];
}else{  $logged_in = false;}

if (isset($_POST['form_server'])){
	$form_server = $_POST['form_server'];
}else{  $form_server = '';}

if (isset($_POST['form_password'])){
	$form_password = $_POST['form_password'];
}else{  $form_password = '';}

if (isset($_POST['form_name'])){
	$form_name = $_POST['form_name'];
	$name = explode('@', $form_name);
	$form_username = $name[0];
	if (isset($name[1])) {$form_domain = $name[1]; $type = 'r_user';}
	if (!isset($name[1])){$type = 's_admin';}
	if (strtolower($name[0]) == 'postmaster') {$type = 'd_admin';}
}else{  $form_name = '';}

$servers = serverlist();
$msg = '';

// check if form is submitted
if (!empty($_POST)) {

	$mail_server->xm_server = $form_server;
	$mail_server->xm_ip = $servers["$form_server"][0];
	$mail_server->xm_port = $servers["$form_server"][1];
	$mail_server->xm_ctrl_t = $type;

	switch ($type) {
		case 's_admin':
			$mail_server->xm_ctrl_p = $form_password;
			$mail_server->xm_ctrl_u = $form_username;
			$mail_server->xm_user_u = $form_username;
			if ($form_password == xmdecrypt($servers["$form_server"][3])) {$logged_in = $mail_server->noop();}
			break;
		default:
			$mail_server->xm_ctrl_p = xmdecrypt($servers["$form_server"][3]);
			$mail_server->xm_ctrl_u = $servers["$form_server"][2];
			$mail_server->xm_user_u = $form_username;
			$mail_server->xm_user_p = $form_password;
			$mail_server->xm_user_d = $form_domain;
			$logged_in = $mail_server->userauth($mail_server->xm_user_d, $mail_server->xm_user_u, $mail_server->xm_user_p);
			break;
	}
	$msg = '<img src=gfx/ico_warn.gif> Your user name or password is not valid for this server<br><br>';
	$_SESSION['mail_server'] = $mail_server;
	$_SESSION['logged_in'] = $logged_in;
}

if ($logged_in != true) {
	echo $msg; ?>
    <form name='frmLogin' action="<?php echo $_SERVER['PHP_SELF']; ?>" method='post'>
        <input type='hidden' name='action' value='login'>
        <table class='tbl'>
            <tr>
                <td><b>Server name:<b></td>
                <td><select name='form_server' style='width: 400px;'>
                    <?php
                    foreach($servers as $svr=>$svr_info) {
                    	$selected = ($svr == $form_server) ? 'selected' : '';
                    	?><option value=<?php echo "'$svr' $selected"; ?>><?php echo $svr; ?></option><?php
                    } ?>
	                </select></td>
            </tr>
            <tr>
                <td><b>User name:*</b></td>
                <td><input type='text' name='form_name' value="<?php echo $form_name; ?>" style='width: 400px;'></td>
            </tr>
            <tr>
                <td><b>Password:</b></td>
                <td><input type='password' name='form_password' style='width: 400px;'  maxlength='100'></td>
            </tr>
            <tr>
                <td colspan="2" align="right"><input type='submit' value=' submit '></td>
            </tr>
        </table>
    </form>

    <hr>
    <img src=gfx/ico_info.gif align=absmiddle title=info>
    * to login as a<br>
    <li><b>user:</b> user name is your email address, password is your POP3 password</li>
	<?php 
	if($cfg_allow_admin_info == "on"){  ?>
        <li><b>domain admin:</b> user name is postmaster@yourdomain, password is your POP3 password</li>
        <li><b>server admin:</b> user name and password are set in your XMail ctrlaccounts.tab</li>
	<?php } ?>
	<!-- Set focus on 1st field... -->
	<script type="text/javascript"><!--
	document.frmLogin.form_name.focus();
	//-->
	</script>
<?php 
} else {
	if (!empty($_SESSION['InfoAreReadable'])){
		$r_user = "Regular user";
		$s_admin = "Server admin";
		$d_admin = "Domain admin";

		echo "<script>parent.menu.location.reload();</script>";
		echo "<img src=gfx/ico_check.gif> Logged in as ${$type} <b>$mail_server->xm_user_u</b> on <b>$mail_server->xm_server</b><br>";
	} else {
		// session cookies are not enabled on client side
		// in certain system it happen also if 3rd party cookies are not enabled
		$_SESSION['logged_in'] = '';
		?>
		<table>
			<tr><td width="20"><img src=gfx/ico_warn.gif></td>
				<td width="300"><font color="Red"><b>
					Login is failed: could not start the session! It's probably due to the privacy settings 
					of the client: either the browser does not permit the use of session cookies (or 3rd party cookies)
					or private header information are removed by a personal firewall.<br><br>
					Fixup the problem and retry please.
					</b></font></td>
				<td width="20"><img src=gfx/ico_warn.gif></td>
			</tr>
		</table>
		</p><hr>
		<form name="frmRetry" action="">
			<input name="retry" id="retry" type="button" value=" Retry... " onclick="LoginRetry()">
		</form>
		<?php
	}
} ?>

