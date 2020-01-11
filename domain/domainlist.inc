
<span class="caption">domains</span><br>
<table class='tbl'>
	<th width="250">name</th>
	<th width="95">users</th>
	<th width="95">mailing lists</th>
	<?php        
	foreach($mail_server->domainlist() as $domain) {
		$users = $mail_server->userlist($domain);
		$U = 0;
		$M = 0;
		foreach ($users as $user){
			if (strtoupper($user[3]) == "U"){
				$U++;
			} else {
				$M++;
			}
		}
		// get mailboxes limit
		$Settings = getDomainSettings($domain);
		if ($Settings['MaxMailboxes'] == 0){
			$MBlimit = '(unlimited)';
		} else {
			$MBlimit = '(max ' .floatval($Settings['MaxMailboxes']) .')';
		}
		unset ($Settings); ?>
	        <tr>
				<td>
				    <a href="main.php?action=domaindel&domain=<?php echo $domain; ?>">
				            <img src='gfx/ico_delete.gif' alt='Delete domain' height='16' width='16' border='0'></a>
				    <a href="main.php?action=userlist&domain=<?php echo $domain; ?>">
				            <img src='gfx/ico_properties.gif' alt='Show properties' height='16' width='16' border='0'>
				            <?php echo $domain; ?>
				    </a>
				</td>
				<td><?php echo $U .' ' .$MBlimit; ?></td>
				<td><?php echo $M; ?></td>
	        </tr>
	<?php } ?>
</table>

<p>&nbsp;</p>

<span class="caption">custom domains</span><br>
<table class='tbl'>
        <th width="250">name</th>
        <th width="200">users</th>
        <?php foreach($mail_server->custdomlist() as $domain) { ?>
        	<tr>
        	<td>
        	<a href="main.php?action=custdomdel&domain=<?php echo $domain; ?>">
        	<img src='gfx/ico_delete.gif' alt='Delete domain' height='16' width='16' border='0'></a>
        	<a href="main.php?action=custdomget&domain=<?php echo $domain; ?>">
        	<img src='gfx/ico_properties.gif' alt='Show properties' height='16' width='16' border='0'>
                                        <?php echo $domain; ?>
                                </a>
                        </td>
                         <td><?php $users = $mail_server->userlist($domain); echo count($users); ?></td>
                </tr>
        <?php } ?>
</table>
<br>
<hr>
<br>

<a href='main.php?action=domainadd'>
        <img src='gfx/ico_create.gif' alt='New domain' height='16' width='16' border='0' valign="top">
        <b>new domain</b>
</a>&nbsp;
<a href='main.php?action=custdomset'>
        <img src='gfx/ico_create.gif' alt='New domain' height='16' width='16' border='0'>
        <b>new custom domain</b>
</a>
