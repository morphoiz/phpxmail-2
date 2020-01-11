<?php

$msg = '';

// check if form is submitted
if (!empty($_POST)) {
	// assign the variables submitted
	foreach ($_POST as $Key=>$Value){
		if (strstr($Key, 'vars_')){
			$filecontentA[] = array ('"' .substr($Key, 5) .'"', '"' .$_POST[$Key] .'"');
		}
	}
	// now store them in the custom file
	if (!SetDomainSettings($domain, $filecontentA)) {
		$msg = '<img src=gfx/ico_warn.gif> Error saving values: ' . $mail_server->xm_err_msg;
	} else {
		$msg = "<img src=gfx/ico_check.gif> Values for the domain settings saved successfully";
	}
}
// read the settings
$filecontent = getDomainSettings($domain);

?>

<form name='frmDomSettings' action='main.php' method='post'>
    <input type='hidden' name='action' value='domsettings'>
    <input type='hidden' name='domain' value='<?php echo $domain; ?>'>
    <?php echo $msg; ?>
    <table border='0' class='tbl'>
    	<?php
    	foreach ($filecontent as $Key=>$value){ ?>
        <tr>
            <td><b><?php echo $Key; ?>:<b></td>
            <td><input type='text' name='vars_<?php echo $Key; ?>' value='<?php echo $value; ?>' size='30' style='width: 300px;'></td>
        </tr>
        <?php } ?>
        <tr>
            <td colspan="2" align="right"><input name='submit' type='submit' value=' submit '></td>
        </tr>
    </table>
</form>
<hr>
<img src=gfx/ico_info.gif align=absmiddle title=info> A zero value in 'MaxMailboxes' means that the quantity is unlimited 
<hr>
<br><br>

<!-- Set focus on 1st field... -->
<script type="text/javascript"><!--
document.frmDomSettings.submit.focus();
//-->
</script>