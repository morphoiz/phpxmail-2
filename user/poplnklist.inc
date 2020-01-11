<?php
	

	$u_link = "domain=$domain&username=$username";

?>

<table class='tbl'>
<?php	foreach($mail_server->poplnklist($domain, $username) as $poplnk) {
	$img = ($poplnk[6] == 'ON') ? 'gfx/ico_on.gif' : 'gfx/ico_off.gif';
	$alt = ($poplnk[6] == 'ON') ? 'link is enabled' : 'link is disabled';
?>
	<tr>
		<td>
			<a href="main.php?action=poplnkdel&<?php echo $u_link; ?>&p_domain=<?php echo $poplnk[2]; ?>&p_username=<?php echo $poplnk[3]; ?>">
				<img src='gfx/ico_delete.gif' alt='delete link' height='16' width='16' border='0'>
			</a>
		<td>
			<img src='<?php echo $img; ?>' alt='<?php echo $alt; ?>'>
			<a href="main.php?action=poplnkdtl&<?php echo $u_link; ?>&p_domain=<?php echo $poplnk[2]; ?>&p_username=<?php echo $poplnk[3]; ?>">
				<?php echo "$poplnk[3]@$poplnk[2]"; ?>
			</a>
<?php } ?>
</table>
<hr>

<a href='main.php?action=poplnkadd&<?php echo $u_link; ?>'>
	<img src='gfx/ico_create.gif' alt='New POP3' height='16' width='16' border='0' valign="top">
	<b>new POP3</b>
</a>

<br><br>
