<?php

$msg = '';

// check if form is submitted
if (!empty($_POST)) {
	if (SpamListAdd($_POST['spamaddr'])){
		$msg = "<img src=gfx/ico_check.gif> Address <b>" .$_POST['spamaddr'] ."</b> successfully added to local spam list.";
	} else {
		$msg = '<img src=gfx/ico_warn.gif> Error adding <b>' .$_POST['spamaddr'] .'</b> to local spam list.'; //.': ' .$mail_server->xm_err_msg;
	}
}

?>

<form name='frmNewSpam' action='main.php' method='post'>
    <input type='hidden' name='action' value='spamadd'>
    <?php echo $msg; ?>
    <table border='0' class='tbl'>
        <tr>
            <td><b>Spam address:<b></td>
            <td><input type='text' name='spamaddr' size='30' style='width: 400px;'></td>
        </tr>
        <tr>
            <td colspan="2" align="right"><input type='submit' value=' submit '></td>
        </tr>
    </table>
</form>
<br>
<hr>
<img src='gfx/ico_info.gif' alt='Info' height='16' width='16' border='0' valign="top"> To block an entire domain use the character <b>*</b> before '@spammerdomain.tld'</p>

<!-- Set focus on 1st field... -->
<script type="text/javascript"><!--
document.frmNewSpam.spamaddr.focus();
//-->
</script>