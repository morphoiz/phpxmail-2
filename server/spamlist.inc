<?PHP

$msg = '';

// check if form is submitted
if (!empty($_GET)) {
	if (isset($_GET['do'])){
		if ($_GET['do'] == "del") {
			if (SpamListDel($_GET['address'])) {
				$msg = "<img src=gfx/ico_check.gif> Address <b>" .$_GET['address'] ."</b> removed successfully.</p>";
			//} else {
				//$msg = '<img src=gfx/ico_warn.gif> Error removing ' .$_GET['address'] .'</p>'; //.': ' .$mail_server->xm_err_msg;
			}
		}
	}
}

echo $msg;
?>

<span class="caption">spam address list</span><br>
<table border=0 class='tbl'>
    <th >&nbsp</th>
    <th >user</th>
    <th >domain</th>
	<?php 
	$SpamAddressList = SpamListGet();
	foreach ($SpamAddressList as $address){
		echo '<tr>';
		echo 	'<td><a href="main.php?action=spamlist&do=del&address=' .$address[0] .'@' .$address[1] .'"> <img src="gfx/ico_delete.gif" border="0" alt="Remove this address"></a>&nbsp;';
		//echo 	'    <a href="main.php?action=spamedit&address=' .$address[0] .'@' .$address[1] .'"> <img src="gfx/ico_see.gif" border="0"></a>';
		echo 	'</td>';
		echo 	'<td align="right">' .$address[0] .'</td>';
		echo 	'<td align="left">@' .$address[1] .'</td>';
		echo '</tr>';
	}
	?>
	<tr><td colspan="3"><hr></td></tr>
	<tr><td colspan="3"><?php echo 'A total of <b>' .count($SpamAddressList) .'</b> addresses are listed.'; ?></td></tr>
</table>
<br>
<hr>
<img src='gfx/ico_info.gif' alt='Info' height='16' width='16' border='0' valign="top"> The character <b>*</b> means that all the e-mails from the <b>whole domain</b> will be blocked.</p>
<a href='main.php?action=spamadd'>
	<img src='gfx/ico_create.gif' alt='New address' height='16' width='16' border='0' valign="top">
    <b>new spam address</b></a>
<br>