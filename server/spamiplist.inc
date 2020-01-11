<?PHP

$msg = '';

// check if form is submitted
if (!empty($_GET)) {
	if (isset($_GET['do'])){
		if ($_GET['do'] == "del") {
			if (SpamIPListDel($_GET['address'])) {
				$msg = "<img src=gfx/ico_check.gif>IP address <b>" .$_GET['address'] ."</b> removed successfully.</p>";
			//} else {
				//$msg = '<img src=gfx/ico_warn.gif> Error removing ' .$_GET['address'] .'</p>'; //.': ' .$mail_server->xm_err_msg;
			}
		}
	}
}

echo $msg;
?>

<span class="caption">spam IP address list</span><br>
<table border=0 class='tbl'>
    <th >&nbsp</th>
    <th width="120">IP</th>
    <th width="120">Mask</th>
    <th width="100">Action</th>
    <th >Delay</th>
	<?php 
	$SpamAddressList = SpamIPListGet();
	foreach ($SpamAddressList as $address){
		echo '<tr>';
		echo 	'<td><a href="main.php?action=spamiplist&do=del&address=' .$address[0] .'"> <img src="gfx/ico_delete.gif" border="0" alt="Remove this IP"></a>&nbsp;';
		echo 	'    <a href="main.php?action=spamipedit&adr=' .$address[0] .'&msk=' .$address[1] .'&act=' .$address[2] .'&dly=' .$address[3] .'"> <img src="gfx/ico_edit.gif" border="0" alt="Edit this IP"></a>';
		echo 	'</td>';
		switch ($address[2]) {
			case 0:
				$font =	'<font color="Red">';
				break;
			case 1:
				$font =	'<font color="Green">';
				break;
			case 2:
				$font =	'<font color="Blue">';
				break;
		}
		echo 	'<td>' .$font .$address[0] .'</font></td>';
		echo 	'<td>' .$font .$address[1] .'</font></td>';
		echo 	'<td>' .$font;
		switch ($address[2]) {
			case 0:
				echo	'always blocked</font>';
				break;
			case 1:
				echo	'<font color="Green">auth SMTP permitted</font>';
				break;
			case 2:
				echo	'<font color="Blue">delayed SMTP</font>';
				break;
		}
		echo 	'</font></td>';
		if ($address[3] !='') $sec = ' sec'; else $sec='';
		echo 	'<td>' .$font .$address[3] .$sec .'</font></td>';
		echo '</tr>';
	}
	?>
	<tr><td colspan="5"><hr></td></tr>
	<tr><td colspan="5"><?php echo 'A total of <b>' .count($SpamAddressList) .'</b> IP addresses are listed.'; ?></td></tr>
</table>
<br>
<hr>
<a href='main.php?action=spamipadd'>
	<img src='gfx/ico_create.gif' alt='New IP address' height='16' width='16' border='0' valign="top">
    <b>new spam IP address</b></a>
<br>