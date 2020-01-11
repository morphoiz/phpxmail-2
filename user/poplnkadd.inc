<?php
	

	$msg = '';

	// check if form is submitted
	if (!empty($_POST)) {
		$enable = $_POST['enable']
			or $enable = 0;
		if ($mail_server->poplnkadd($domain, $username, $_POST['p_domain'], $_POST['p_username'], $_POST['p_password'], $_POST['authtype'])
				&& $mail_server->poplnkenable($enable, $domain, $username, $_POST['p_domain'], $_POST['p_username'])) {
			$msg = '<img src=gfx/ico_check.gif> POP3 link saved successfully';
		} else {
			$msg = '<img src=gfx/ico_warn.gif> Error adding POP3 link: ' . $mail_server->xm_err_msg;
		}
	}

?>

<form name="frmPOP3" action="" method="post">
	<input type='hidden' name='action' value='poplnkadd'>
	<input type='hidden' name='domain' value='<?php echo $domain; ?>'>
	<input type='hidden' name='username' value='<?php echo $username; ?>'>
	<?php echo $msg; ?>
	<table class='tbl'>
		<tr>
			<th width="150">
			<th width="*">
		<tr>
			<td><b>remote username:<b>
			<td>
				<input type='text' name='p_username' size='30' style='width: 400px;'>
		<tr>
			<td><b>pop3 server:<b>
			<td>
				<input type='text' name='p_domain' size='30' style='width: 400px;'>
		<tr>
			<td><b>password:<b>
			<td>
				<input type='text' name='p_password' size='30' style='width: 400px;'>
		<tr>
			<td><b>authentication:<b>
			<td>
				<input type='radio' name='authtype' value='CLR' checked> CLR
				<input type='radio' name='authtype' value='APOP'> APOP
		<tr>
			<td><b>enabled:<b>
			<td>
				<input type='checkbox' name='enable' value='1' checked>
		<tr>
			<td colspan="2" align="right"><input type='submit' value=' submit '>
	</table>
</form>
<hr>
<br><br>
