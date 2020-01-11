<?php


        $msg = '';
        $vars = '';
        $allowed = array('RealName', 'HomePage', 'Address', 'Telephone');

        // check if form is submitted
        if (!empty($_POST)) {
                // change password if needed
                if ($_POST['ch_pass']) {
                        if ($mail_server->userpasswd($domain, $mail_server->xm_user_u, $_POST['password'])) {
                                $msg = "<img src=gfx/ico_check.gif> password for <b>$mail_server->xm_user_u@$domain</b> changed successfully.<br>";
                        } else {
                                $msg = '<img src=gfx/ico_warn.gif> Error changing password: ' . $mail_server->xm_err_msg;
                                break 2;
                        }
                }
                // save user vars
                foreach($_POST as $var=>$val) { // loop through all vars
                        if (preg_match("var_", $var)) {
                                $var = substr($var, 4);
                                //if ($val == '') $val = '.|rm'; // remove value
                                if ($val == '') $val = '??'; // user cannot remove the variable
                                $vars .= "$var\t$val\t";
                        }
                }
                if ($mail_server->uservarsset($domain, $mail_server->xm_user_u, $vars)) {
                        $msg .= "<img src=gfx/ico_check.gif> Values for <b>$mail_server->xm_user_u@$domain</b> saved successfully";
                } else {
                        $msg .= '<img src=gfx/ico_warn.gif> Error saving values: ' . $mail_server->xm_err_msg;
                }
        } // end if

        $usr = $mail_server->userlist($domain, $mail_server->xm_user_u);
        $password = $usr[0][2];
        $uservars = $mail_server->uservars($domain, $mail_server->xm_user_u);

?>

<form name='frmUserDetails' action='main.php' method='post'>
        <input type='hidden' name='action' value='userchange'>
        <input type='hidden' name='domain' value='<?php echo $domain; ?>'>
        <input type='hidden' name='ch_pass' value=''>
        <?php echo $msg; ?>
        <table>
                <tr>
                        <th width="150">
                        <th width="*">
                <tr>
                        <td><b>user name:<b>
                        <td><?php echo $mail_server->xm_user_u; ?>
                <tr>
                        <td><b>password:</b>
                        <td><input type='password' name='password' size='30' style='width: 400px;' value='<?php echo $password; ?>' onChange='ch_pass.value = true'>
        <?php foreach ($uservars as $key => $value) {
                if ($key == 'MaxMBSize') $value = humansize($value * 1024);
                if ($key == 'MaxMessageSize') $value = humansize($value * 1024);
        ?>
                <tr>
                        <td><b><?php echo $key; ?><b>
                        <td>
                        <?php if (in_array($key, $allowed)) { ?>
                                <input type='text' name='var_<?php echo $key; ?>' size='30' style='width: 400px;' value='<?php echo $value; ?>'>
                        <?php } else {
                                echo $value;
                        } ?>
        <?php } ?>
                <tr>
                        <td colspan="2" align="right"><input type='submit' name='save' value=' Save values '>
        </table>
</form>

<br>
<span class="caption">statistic data</span>
<table>
        <tr>
                <th width="150">
                <th width="*">
<?php foreach($mail_server->userstats($domain, $mail_server->xm_user_u) as $key => $value) {
        if ($key == 'MailboxSize') {
                $value2 = sprintf("%d%% of %s", (($value/1024)/$uservars["MaxMBSize"])*100, humansize($uservars['MaxMBSize'] * 1024));
                $value = humansize($value);
        }
?>
        <tr>
                <td><b><?php echo "$key";?></b>
                <td>
                        <?php echo "$value";?>
                        <?php if (isset($value2)) { echo " ($value2)"; unset($value2); } ?>
<?php } ?>
</table>
<hr>
<br><br>