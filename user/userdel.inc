<?php


        $msg = '';

        // check if form is submitted
        if (!empty($_POST)) {
                if (isset($_POST['yes'])) {
                        if ($mail_server->userdel($domain, $username)) {
                                $msg = "<img src=gfx/ico_check.gif> User <b>$username@$domain</b> is deleted successfully.";
                        } else {
                                $msg = '<img src=gfx/ico_warn.gif> Error deleting user: ' . $mail_server->xm_err_msg;
                        }
                } else {
                        $msg = "<img src=gfx/ico_stop.gif> User <b>$username@$domain</b> is <b>NOT</b> deleted";
                }
        }

?>

<?php if (empty($_POST)) { ?>

        <p>You are about to delete mail user <b><?php echo "$username@$domain"; ?></b>.</p>
        <p>Do you really want to delete this user?</p>

        <form action='main.php' method='post'>
                <input type='hidden' name='action' value='userdel'>
                <input type='hidden' name='domain' value='<?php echo $domain; ?>'>
                <input type='hidden' name='username' value='<?php echo $username; ?>'>
                <input type='submit' name='yes' style='width: 50px;' value=' Yes '>
                <input type='submit' name='no' style='width: 50px;' value=' No '>
        </form>

<?php } else { ?>

        <p><?php echo $msg; ?></p>

        <form action='main.php' method='post'>
                <input type='hidden' name='action' value='userlist'>
                <input type='hidden' name='domain' value='<?php echo $domain; ?>'>
                <input type='submit' name='click' style='width: 50px;' value=' OK '>
        </form>

<?php } ?>