<?php

$msg = '';

// check if the access to this Mailing List is unrestricted
$uservars = $mail_server->uservars($domain, $mlname);
if (key_exists('ClosedML', $uservars)) {
	$Unrestricted = ($uservars['ClosedML'] == 1);
} else {
	$Unrestricted = false;
}
?>

<table>
	<tr>
		<th class="med-blue" align="left" width="250">name</th>
		<th class="med-blue" align="left" width="175">rights</th>
	</tr>
	<?php 
	foreach($mail_server->mluserlist($domain, $mlname) as $mailaddress) { ?>
		<tr>
			<td>
				<a href="main.php?action=mlusrchgdel&domain=<?php echo $domain; ?>&mlname=<?php echo $mlname; ?>&mailaddress=<?php echo $mailaddress[0]; ?>">
					<img src='gfx/ico_delete.gif' alt='Remove user' height='16' width='16' border='0'></a>
				<?php 
				if ($Unrestricted) {
					echo '<a href="main.php?action=mlusrchgedit&domain=' .$domain .'&mlname=' .$mlname .'&mailaddress=' .$mailaddress[0] .'&parm=' .$mailaddress[1] .'">';
					echo $mailaddress[0] .'</a></td>';
				} else {
					echo $mailaddress[0];
				}
				?>
			<td>
				<?php 
				if ($Unrestricted) {
					$rights = str_replace("R","Read  ", $mailaddress[1]);
					$rights = str_replace("W","Write  ", $rights);
					$rights = str_replace("A","Write (auth)", $rights);
					echo str_replace("  "," + ",trim($rights));
				} else {
					echo '(unrestricted)';
				}
				?></td>
		</tr>
	<?php } ?>
</table>
<hr>

<a href="main.php?action=mlusrchgadd&domain=<?php echo $domain; ?>&mlname=<?php echo $mlname; ?>">
	<img src='gfx/ico_user_new.gif' alt='New user' height='16' width='16' border='0'>
	<b>Add new user</b>
</a></p>
<hr>

<table>
<tr><td><img src='gfx/ico_info.gif' alt='info'></td><td>clicking on the cross icon you will remove the user from the list</td></tr>
<?php if ($Unrestricted) {?>
	<tr><td><img src='gfx/ico_info.gif' alt='info'></td><td>clicking on the user address you may modify rights for that user</td></tr>
<?php } else {?>
	<tr><td><img src='gfx/ico_notice.gif' alt='info'></td><td>To enable user rights restrictions, <B>'ClosedML'</B> must be set to <b>'1'</b><br>in <b><?php echo $mlname ?></b> mailing list account</td></tr>
<?php } ?>
</table>
<br>
