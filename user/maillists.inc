<?php


// get all users
$all = $mail_server->userlist($domain);

// split in users and lists
$lists = array();
foreach ($all as $id => $user) {
	if (strtoupper($user[3]) == 'M') $lists[] = $user;
}

unset($all);

?>

<span class="caption">mailing lists</span>
<table border=0 class='tbl'>
    <th width="250">name
    <th width="175">members
    <?php  foreach($lists as $list) { ?>
        <tr>
            <td>
                <a href="main.php?action=mllistdel&domain=<?php echo $domain; ?>&username=<?php echo $list[1]; ?>">
                    <img src='gfx/ico_delete.gif' alt='Delete user' height='16' width='16' border='0'></a>
                <a href="main.php?action=mluservars&domain=<?php echo $domain; ?>&mlname=<?php echo $list[1]; ?>">
                    <img src="gfx/ico_list.gif" alt='Show subscribers' height='16' width='16' border='0'>
                    <?php echo $list[1]; ?></a><td>
                    <?php $n=0; foreach($mail_server->mluserlist($domain, $list[1]) as $mailaddress) {$n++;} ?>
                    <?php echo $n; ?>
    <?php } ?>
    
</table>
<br>
<hr>
<br>

<a href='main.php?action=mllistadd&usertype=M&domain=<?php echo $domain; ?>'>
         <img src='gfx/ico_list_new.gif' alt='New Mailing list' height='16' width='16' border='0'>
         <b>new mailing list</b>
</a>