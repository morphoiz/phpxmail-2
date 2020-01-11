<?php


$msg = '';
$p_username = $_REQUEST['p_username'];
$p_domain = $_REQUEST['p_domain'];
$poplnk = array_shift($mail_server->poplnklist($domain, $username));

// check if form is submitted
if (!empty($_POST)) {
	$enable = empty($_POST['enable']) ? 0 : $_POST['enable'];
	if ($mail_server->poplnkdel($domain, $username, $p_domain, $p_username)
	&& $mail_server->poplnkadd($domain, $username, $p_domain, $p_username, $_POST['p_password'], $_POST['authtype'])
	&& $mail_server->poplnkenable($enable, $domain, $username, $p_domain, $p_username)) {
		$msg = '<img src=gfx/ico_check.gif> POP3 link saved successfully';
	} else {
		$msg = '<img src=gfx/ico_warn.gif> Error saving POP3 link: ' . $mail_server->xm_err_msg;
	}
}

foreach($mail_server->poplnklist($domain, $username) as $entry) {
	if (in_array($p_domain, $entry) && in_array($p_username, $entry)) $poplnk = $entry;
}

?>

<form name="frmPOP3" action="" method="post">
	<input type='hidden' name='action' value='poplnkdtl'>
	<input type='hidden' name='domain' value='<?php echo $domain; ?>'>
	<input type='hidden' name='username' value='<?php echo $username; ?>'>
	<input type='hidden' name='p_domain' value='<?php echo $p_domain; ?>'>
	<input type='hidden' name='p_username' value='<?php echo $p_username; ?>'>
	<?php echo $msg; ?>
	<table class='tbl'>
		<tr>
			<th width="150">
			<th width="*">
		<tr>
			<td><b>remote username:<b>
			<td><?php echo $poplnk[3]; ?>
		<tr>
			<td><b>pop3 server:<b>
			<td><?php echo $poplnk[2]; ?>
		<tr>
			<td><b>password:<b>
			<td><input type='password' name='p_password' size='30' style='width: 400px;' value='<?php echo $poplnk[4]; ?>'>
		<tr>
			<td><b>authentication:<b>
			<td>
				<input type='radio' name='authtype' value='CLR' <?php if ($poplnk[5] == 'CLR') echo 'checked'; ?>> CLR
				<input type='radio' name='authtype' value='APOP' <?php if ($poplnk[5] == 'APOP') echo 'checked'; ?>> APOP
		<tr>
			<td><b>enabled:<b>
			<td><input type='checkbox' name='enable' value='1' <?php if ($poplnk[6] == 'ON') echo 'checked'; ?>>
		<tr>
			<td colspan="2" align="right"><input type='submit' value=' save '>
	</table>
</form>
<hr>
<br><br>
