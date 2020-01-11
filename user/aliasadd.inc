<?php

$msg = '';

// check if form is submitted
if (!empty($_POST)) {
	$alias = $_POST['alias'];
	//avoid use of catchall alias for regular users
	if ($alias == '*' && $mail_server->xm_ctrl_t == 'r_user'){
		$msg = '<img src=gfx/ico_warn.gif> Error adding alias: <b>*</b> is not a valid alias';
	} else {
		$at = strpos($alias, '@');
		if($at) {
			$alias_domain = substr($alias, $at + 1);
			$alias_alias = substr($alias, 0, $at);
			$alias_username = $_POST['username'] .'@'. $domain;
		} else {
			$alias_domain = $domain;
			$alias_alias = $_POST['alias'];
			$alias_username = $_POST['username'];
		}
		//check that configuration allows crossdomain aliases
		if ($domain != $alias_domain){
			if ($mail_server->xm_ctrl_t == 'r_user'){
				//  regular user cannot add Cross Domain Aliases by himself...
				$msg = '<img src=gfx/ico_warn.gif> Cross domain aliases are not allowed';
			} else {
				if ($cfg_allow_cross_alias == "off"){
					$msg = '<img src=gfx/ico_warn.gif> Cross domain aliases are disabled in the configuration';
				}
			}
		}
		if ($msg == ''){
			if ($mail_server->aliasadd($alias_domain, $alias_alias, $alias_username)) {
				$msg = "<img src=gfx/ico_check.gif> New alias <b>$alias_alias@$alias_domain</b> created successfully.";
			} else {
				$msg = '<img src=gfx/ico_warn.gif> Error adding alias: ' . $mail_server->xm_err_msg;
			}
		}
	}
}

//check for Alias limits
$Settings = getDomainSettings($domain);
$MaxAlias = $Settings['MaxAliasPerUser'];
unset ($Settings);
$ButtonEnabling = '';
//if ($MaxAlias > 0){
if ($mail_server->xm_ctrl_t != 's_admin') {
	$Aliases = count($mail_server->aliaslist($domain, '*', $username));
	$Aliases += count($mail_server->aliaslist('*', '*', $username .'@'. $domain));
	if ($Aliases >= $MaxAlias) $ButtonEnabling = 'disabled';
}
?>

<form name='frmNewAlias' action='main.php' method='post'>
    <input type='hidden' name='action' value='aliasadd'>
    <input type='hidden' name='domain' value='<?php echo $domain; ?>'>
    <input type='hidden' name='username' value='<?php echo $username; ?>'>
    <?php echo $msg; ?>
    <table border='0' class='tbl'>
        <tr>
            <td><b>alias:<b></td>
            <td><input type='text' name='alias' size='30' style='width: 400px;'></td>
        </tr>
        <tr>
            <td colspan="2" align="right"><input type='submit' <?php echo $ButtonEnabling ?> value=' submit '></td>
        </tr>
    </table>
</form>
<hr>
<img src=gfx/ico_info.gif align=absmiddle title=info> There is no need to postplace @<?php echo $domain; ?> to the user name
<?php 	if (!($mail_server->xm_ctrl_t == 'r_user')){ ?>
<br>
<img src=gfx/ico_info.gif align=absmiddle title=info> Inserting * character will make this a 'catch-all' type mailbox.
<?php } ?>
<hr>
<br>
<?php
if ($ButtonEnabling != '') {
	echo "<img src=gfx/ico_notice.gif>The max number of aliases has been reached!<br>";
}
?>
<br>

<!-- Set focus on 1st field... -->
<script type="text/javascript"><!--
document.frmNewAlias.alias.focus();
//-->
</script>