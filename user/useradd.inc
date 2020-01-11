<?php

$msg = '';

// check if form is submitted
if (isset($_POST['submit'])) {
	if ($_POST['username'] && $_POST['password']){
		if ($mail_server->useradd($domain, $_POST['username'], $_POST['password'], $_POST['usertype'])) {
			if (strtoupper($_POST['usertype']) == "U" ){$type = "user";}
			if (strtoupper($_POST['usertype']) == "M" ){$type = "mailing list";}
			$msg = "<img src=gfx/ico_check.gif> New $type <b>$_POST[username]@$domain</b> created successfully.";
		} else {
			$msg = '<img src=gfx/ico_warn.gif> Error adding user: ' . $mail_server->xm_err_msg;
		}
	} else {
		$msg = "<img src=gfx/ico_warn.gif> Please fill in the fields of the form.";
	}
}

if (isset($_POST['usertype'])){$usertype = $_POST['usertype'];}
if (isset($_GET['usertype'])){$usertype =  $_GET['usertype'];}

//check for limits
$Settings = getDomainSettings($domain);
$MaxMB = $Settings['MaxMailboxes'];
unset ($Settings);
$ButtonEnabling = '';
$i=0;
if ($MaxMB > 0 && strtoupper($usertype) == "U"){
	$users = $mail_server->userlist($domain);
	foreach ($users as $user){
		if ($user[3] == $usertype) $i++;
	}
	if ($i >= $MaxMB) $ButtonEnabling = 'disabled';
}

?>

<form name='frmNewUser' action='main.php' method='post'>
        <input type='hidden' name='action' value='<?php if (strtoupper($usertype) == 'M') echo "mllistadd"; else echo "useradd"; ?>'>
        <input type='hidden' name='domain' value='<?php echo $domain; ?>'>
        <input type='hidden' name='usertype' value='<?php echo $usertype; ?>'>
        <?php echo $msg; ?>
        <table border='0' class='tbl'>
            <tr>
                <td><b>User name:<b>
                <td><input type='text' name='username' size='30' style='width: 400px;'>
            <tr>
                <td><b>User password:</b>
                <td><input type='password' name='password' size='30' style='width: 400px;'>
            <tr>
                <td colspan="2" align="right"><input type='submit' <?php echo $ButtonEnabling ?> name='submit' value=' submit '>
        </table>
</form>
<hr>
<br>
<?php
if ($ButtonEnabling != '') {
	echo "<img src=gfx/ico_notice.gif>The max number of users has been reached!<br>";
}
?>
<br>

<!-- Set focus on 1st field... -->
<script type="text/javascript"><!--
document.frmNewUser.username.focus();
//-->
</script>
