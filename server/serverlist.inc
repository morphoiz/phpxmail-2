<?php
	

	$servers = serverlist();

?>

<span class="caption">servers</span>
<table>
	<th width='250' align="left">server name
	<th width='150' align="left">address
	<th width='50' align="left">port
	<th width='50' align="left">ssh
	<?php	foreach($servers as $svr => $svr_info) { ?>
		<tr>
			<td>
				<a href="main.php?action=serverdel&svr=<?php echo $svr; ?>">
					<img src='gfx/ico_delete.gif' alt='Remove server' height='16' width='16' border='0'></a>
				<a href="main.php?action=servercfg&svr=<?php echo $svr; ?>">
					<img src='gfx/ico_properties.gif' alt='Edit properties' height='16' width='16' border='0'>
					<?php echo $svr; ?></a>
			<td><?php echo $svr_info[0]; ?>
			<td><?php echo $svr_info[1]; ?>
			<td><?php echo ($svr_info[4]) ? 'yes' : 'no'; ?>
	<?php } ?>
</table>
<br>
<hr>
<br>
<a href='main.php?action=serveradd'>
	<img src='gfx/ico_create.gif' alt='New server' height='16' width='16' border='0'>
	<b>Add new server</b>
</a>

<br><br>
