<?php
	

	$msg = '';
	$mlname = $_REQUEST['mlname'];
	$mailaddress = $_REQUEST['mailaddress'];
	$m_link = "domain=$domain&mlname=$mlname";

	if (!isset($mailaddress)) { // recipient is list
		$list = TRUE;
		$to_addr = "$mlname@$domain";
	} else {
		$list = FALSE;
		$to_addr = $mailaddress;
	}
	
	// check if form is submitted
	if (!empty($_POST)) {
		$message = stripslashes($_POST['message']);
		//include('mime_mail.inc');
		$msg = "Sent mail to $to_addr successfully";
	}

	echo '<span class="caption">Send mail to: </span><b>' .$to_addr .'</b>';
	if ($list) echo ' (whole mailing list)';
	echo '</p>';
?>

<script language="javascript" type="text/javascript">
function CheckHTML()
{
	popWidth=500;
	popHeight=350;
	strHTML=frmMail.message.value;
	popup = window.open("checkhtml.php?strHTML=" + strHTML,"popDialog2",
			"width=" + popWidth + ", height=" + popHeight + ", left=10, top=10, titlebar=no, scrollbars=yes");
}
</script>

<form name='frmMail' action='main.php' method='post'>
	<input type='hidden' name='action' value='sendmail'>
	<input type='hidden' name='domain' value='<?php echo $domain; ?>'>
	<input type='hidden' name='mlname' value='<?php echo $mlname; ?>'>
	<input type='hidden' name='mailaddress' value='<?php echo @$mailaddress; ?>'>
	<input type='hidden' name='to_addr' value='<?php echo $to_addr; ?>'>
	<?php echo $msg; ?>
	<table>
		<tr>
			<td><b>From address:<b>
			<td>
				<?php	if ($list) { ?>
					<select name='from_addr' style='width: 400px;'>
					<?php foreach($mail_server->mluserlist($domain, $mlname) as $mluser) {
						if (strstr($mluser[1], 'W')) //only users with write perms
							echo "<option value='$mluser[0]'>$mluser[0]<br>\n";
					} ?>
					</select>
				<?php } else { ?>
					<input type='text' name='from_addr' size='30' style='width: 400px;'>
				<?php } ?>
		<tr>
			<td><b>From name:<b>
			<td><input type='text' name='from_name' size='30' style='width: 400px;'>
		<tr>
			<td><b>Subject:<b>
			<td><input type='text' name='subject' size='30' style='width: 400px;'>
		<tr>
			<td valign="top"><b>Message:<b>
			<td>
				<textarea name='message' rows="10" cols="50" style='width: 400px;'></textarea>
		<tr>
			<td colspan="2" align="right">
				<input type='button' value=' test html ' onClick="CheckHTML()">
				<input type='submit' value=' submit '>
	</table>
</form>
<hr>

<img src='gfx/ico_info.gif' alt=''>
You can use HTML in the message to include links, images or to format text.<br>
This requires a basic knowledge of HTML.<br>
You can check the message, but this is not 100% guaranteed!<br>
For sending mail to lists, there must be at least one user on that list with write permissions!<br>  <!-- changes -->

<br><br>
