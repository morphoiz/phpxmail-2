<?php


        $msg = '';

        // check if form is submitted
        if (isset($_POST['submit'])) {
            if($_POST['domain']){
                $domain = $_POST['domain'];
                if ($mail_server->custdomset($domain)) {
                        $msg = "<img src=gfx/ico_check.gif> New custom domain <b>$domain</b> created successfully.<br>";
                } else {
                        $msg = '<img src=gfx/ico_warn.gif> Error adding custom domain: ' . $mail_server->xm_err_msg;
                }
            }else{
                    $msg = "<img src=gfx/ico_warn.gif> Please fill in the fields of the form.";
            }
        }

?>

<form name='frmNewDomain' action='main.php' method='post'>
        <input type='hidden' name='action' value='custdomset'>
        <?php echo $msg; ?>
        <table border='0' class='tbl'>
                <tr>
                        <td><b>Custom Domain Name:<b>
                        <td><input type='text' name='domain' size='30' style='width: 400px;' value='<?php echo $domain; ?>'>
                <tr>
                        <td colspan="2" align="right"><input type='submit' name='submit' value=' submit '>
        </table>
</form>
<hr>
<br><br>

<!-- Set focus on 1st field... -->
<script type="text/javascript"><!--
document.frmNewDomain.domain.focus();
//-->
</script>