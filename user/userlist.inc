<?php

// get all users
$all = $mail_server->userlist($domain);
$users = array();
$stat_space = 0;
foreach ($all as $id => $user) {
	if (strtoupper($user[3]) != 'M') {
		if ($cfg_userlist_stat == "on") {
			foreach($mail_server->userstats($domain, $user[1]) as $key => $value){
				if ($key == 'MailboxSize') {
					array_push($user, humansize($value));
					$stat_space = $stat_space + $value;
				}
				if ($key == 'MailboxMessages') {array_push($user, $value);}
				if ($key == 'LastLoginTimeDate') {
					if (time() - strtotime($value) > 2) {
						array_push($user, date('d/m/Y H:i:s', strtotime($value)));
					} else {
						array_push($user, '-');
					}
				}
				if ($key == 'LastLoginIP') array_push($user, str_replace('0.0.0.0', "-", $value));
			}
		}
		$users[] = $user;
	}
}
unset($all);

//check if exists a catch-all mailbox
$catchall = '';
foreach ($mail_server->aliaslist($domain) as $catch) {
	if ($catch[1] == '*') {$catchall = $catch[2];}
}
//set icon
foreach($users as $n => $user) {
	if ($user[1] == $catchall) {
		$users[$n][9]="ico_catch.gif";
	}elseif($user[1]=='postmaster' || $user[1]=='hostmaster' || $user[1]=='webmaster') {
		$users[$n][9]="ico_postmaster.gif";
	}else{
		$users[$n][9]="ico_user.gif";
	}
}

// get mailboxes limit
$Settings = getDomainSettings($domain);
$MaxMB = $Settings['MaxMailboxes'];
unset ($Settings);
if ($MaxMB == 0){
	$MBlimit = '(unlimited)';
} else {
	$MBlimit = '(max: ' .floatval($MaxMB) .')';
}

?>

<span class="caption">users</span>
<table border=0 class='tbl'>
       <?php 
       echo '<th width="240">name';
       if ($cfg_userlist_stat == "on") {
       	echo '<th width="40" align="center">msgs</th>
       				<th width="50">size</th> 
       				<th width="150">last access (gmt)</th>
       				<th width="110">last IP</th>';
       }
       		foreach($users as $user) {  ?>
                <tr><td>
                <a href="main.php?action=userdel&domain=<?php echo $domain; ?>&username=<?php echo $user[1]; ?>">
                <img src='gfx/ico_delete.gif' alt='Delete user' height='16' width='16' border='0'></a>
                <a href="main.php?action=uservars&domain=<?php echo $domain; ?>&username=<?php echo $user[1]; ?>">
                <img src="gfx/<?php echo $user[9] ?>" alt='Show properties' height='16' width='16' border='0'>
                <?php echo $user[1]; ?></a>
                <td align="center"><?php if ($cfg_userlist_stat == "on") echo @$user[5]; ?></td>
                <td><?php if ($cfg_userlist_stat == "on") echo @$user[4]; ?></td>
                <td><?php if ($cfg_userlist_stat == "on") echo @$user[6]; ?></td>
                <td><?php if ($cfg_userlist_stat == "on") echo @$user[7]; ?></td></tr>
       <?php } 
       echo '<tr><td colspan=5><hr></td></tr><tr>';
       echo '<td> Total: ' .count($users) .'&nbsp;users ' .$MBlimit .'</td>';
       if ($cfg_userlist_stat == "on") {
       	echo '<td>&nbsp;</td>';
       	echo '<td>' .humansize($stat_space) .'</td>';
       		} ?>
       		</tr>
</table>

<br>
<hr>
<br>
<?php
if ($MBlimit == '(unlimited)' || $MaxMB > count($users)){ ?>
<a href='main.php?action=useradd&usertype=U&domain=<?php echo $domain; ?>'>
         <img src='gfx/ico_user_new.gif' alt='New user' height='16' width='16' border='0'>
         <b>new user</b>
</a>
<?php } else 
		echo "<img src=gfx/ico_notice.gif>The max number of users has been reached!<br>";
?>