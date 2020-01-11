<?php


        $msg = '';

        // check if form is submitted
        if (isset($_POST['submit'])) {
            if($_POST['domain'] && $_POST['password']){
                $domain = $_POST['domain'];
                $password = $_POST['password'];
                if ($mail_server->domainadd($domain)
                                && $mail_server->useradd($domain, 'postmaster', $password, 'U')) {
                        $msg  = "<img src=gfx/ico_check.gif> New domain <b>$domain</b> created successfully.<br>";
                        $msg .= "<img src=gfx/ico_check.gif> New user <b>postmaster@$domain</b> created successfully.<br>";
                } else {
                        $msg = '<img src=gfx/ico_warn.gif> Error adding domain: ' . $mail_server->xm_err_msg;
                }
            }else{
                    $msg = "<img src=gfx/ico_warn.gif> Please fill in the fields of the form.";
            }
        }

?>

<form name='frmNewDomain' action='main.php' method='post'>
        <input type='hidden' name='action' value='domainadd'>
        <?php echo $msg; ?>
        <table border='0' class='tbl'>
                <tr>
                        <td><b>Domain Name:<b>
                        <td><input type='text' name='domain' size='30' style='width: 400px;' >
                <tr>
                        <td><b>Postmaster password:</b>
                        <td><input type='password' name='password' size='30' style='width: 400px;' >
                <tr>
                        <td colspan="2" align="right"><input type='submit' name='submit' value=' submit '>
        </table>
</form>
<hr>
<br><br>