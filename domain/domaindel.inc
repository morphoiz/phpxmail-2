<?php


        $msg = '';

        // check if form is submitted
        if (!empty($_POST)) {
                if (isset($_POST['yes'])) {
                        $msg = '<img src=gfx/ico_warn.gif> Error deleting domain: ' . $mail_server->xm_err_msg;
                        switch ($action) {
                                case 'domaindel':
                                        if ($mail_server->domaindel($domain))
                                                $msg = "<img src=gfx/ico_check.gif> Domain <b>$domain</b> deleted successfully.";
                                        break;
                                case 'custdomdel':
                                        if ($mail_server->custdomset($domain, ".\n"))
                                                $msg = "<img src=gfx/ico_check.gif> Custom domain <b>$domain</b> was deleted successfully.<br>";
                                        break;
                        }
                } else {
                        $msg = "<img src=gfx/ico_stop.gif> Domain <b>$domain</b> is <b>NOT</b> deleted.";
                }
        }

?>

<?php if (empty($_POST)) { ?>

        <?php if ($action == 'domaindel') { ?>
                <p>You are about to delete mail domain <b><?php echo $domain; ?></b>.</p>
                <p>This domain has the following users:</p>
                <?php foreach($mail_server->userlist($domain) as $user) {
                        echo "<li>$user[1]@$domain";
                } ?>
                <p>Do you really want to delete this domain and all users?</p>
        <?php } else { ?>
                <p>You are about to delete custom domain <b><?php echo $domain; ?></b>.</p>
                <p>Do you really want to delete this domain?</p>
        <?php } ?>

        <form action='main.php' method='post'>
                <input type='hidden' name='action' value='<?php echo $action; ?>'>
                <input type='hidden' name='domain' value='<?php echo $domain; ?>'>
                <input type='submit' name='yes' style='width: 50px;' value=' Yes '>
                <input type='submit' name='no' style='width: 50px;' value=' No '>  
        </form>

<?php } else { ?>

        <p><?php echo $msg; ?></p>

        <form action='main.php' method='post'>
                <input type='hidden' name='action' value='domainlist'>
                <input type='hidden' name='domain' value='<?php echo $domain; ?>'>
                <input type='submit' name='click' style='width: 50px;' value=' OK '>
        </form>

<?php } ?>