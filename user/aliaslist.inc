<?php

$u_link = "domain=$domain&username=$username";

// get alias limit
$Settings = getDomainSettings($domain);
$MaxAliases = $Settings['MaxAliasPerUser'];
unset ($Settings);
if ($MaxAliases == 0){
	$Aliaseslimit = '(allowed: none)';
} else {
	$Aliaseslimit = '(allowed: ' .floatval($MaxAliases) .')';
}

?>
<table class='tbl'>
	<tr><th colspan="2">Aliases</th></tr>
	<?php
	$A = 0;
	foreach($mail_server->aliaslist($domain, '*', $username) as $alias) { ?>
        <tr>
			<td>
            <a href="main.php?action=aliasdel&<?php echo $u_link; ?>&alias=<?php echo $alias[1]; ?>">
         		<img src='gfx/ico_delete.gif' alt='delete alias' height='16' width='16' border='0'>
            </a></td>
            <td><?php echo "<b>$alias[1]@$alias[0]</b>"; if ($alias[1]  == '*') echo "&nbsp;(catch-all alias)"; ?></td>
    	</tr>
	<?php 
	$A++;
	}
    foreach($mail_server->aliaslist('*', '*', $username .'@'. $domain) as $alias) { ?>
        <tr>
            <td>
            <a href="main.php?action=aliasdel&<?php echo $u_link; ?>&alias=<?php echo "$alias[1]@$alias[0]"; ?>">
                <img src='gfx/ico_delete.gif' alt='delete alias' height='16' width='16' border='0'>
			</a></td>
            <td><?php echo "<b>$alias[1]@$alias[0]</b>"; if ($alias[0] != $domain) echo "&nbsp;(cross domain alias)"; ?></td>
      	</tr>
	<?php 
	$A++;
    }
    echo '<tr><td colspan=2><hr></td></tr>';
    echo '<tr><td colspan=2> Total: ' .$A .'&nbsp;aliases ' .$Aliaseslimit .'</td></tr>';
	?>
</table>
<hr>
<br>
<?php
if (($MaxAliases > $A) || ($mail_server->xm_ctrl_t == 's_admin')) { ?>
<a href='main.php?action=aliasadd&<?php echo $u_link; ?>'>
        <img src='gfx/ico_create.gif' alt='New alias' height='16' width='16' border='0' valign="top">
        <b>new alias</b>
</a>
<?php } else 
echo "<img src=gfx/ico_notice.gif>Cannot add any more aliases!<br>";
?>
<br>