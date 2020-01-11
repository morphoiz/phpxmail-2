<?php


$msg = '';

// check if form is submitted
if (!empty($_POST)) {
	$filename = strtolower(trim($_POST['filename']));
	// do not accept '......' (dots only) as filename
	if (strspn($filename, '.') == strlen($filename)) unset($filename);
	if (isset($filename) && strlen($filename) > 0){
		// file suffix must be ".TAB"
		if (strstr($filename, '.tab') != '.tab') $filename .= '.tab';
		if (!$mail_server->cfgfileget('filters/' .$filename)){
			$dummy = array();
			$dummy[]='';
			if ($mail_server->cfgfileset('filters/' .$filename, $dummy)) {
				$msg = "<img src=gfx/ico_check.gif> New filter <b>'" .$filename ."'</b> created successfully.";
			} else {
				$msg = '<img src=gfx/ico_warn.gif> Error adding filter file: ' . $mail_server->xm_err_msg;
			}
		} else {
			$msg = "<img src=gfx/ico_stop.gif> File <b>'" .$filename ."'</b> already exists.";
		}
	} else {
		$msg = "<img src=gfx/ico_warn.gif> Please fill in the field of the form.";
	}
}

if (empty($_POST)) {
?>

<form name='frmNewFile' action='' method='post'>
    <input type='hidden' name='action' value='filteradd'>
    <?php echo $msg; ?>
    <table border='0' class='tbl'>
        <tr>
            <td><b>File name:<b>
            <td><input type='text' name='filename' size='30' style='width: 400px;'>
        <tr>
            <td colspan="2" align="right"><input type='submit' name='submit' value=' submit '>
    </table>
</form>
<hr>
<?php } else { ?>

	<p><?php echo $msg; ?></p>

	<form action='main.php' method='post'>
		<input type='hidden' name='action' value='filterlist'>
		<input type='submit' name='click' style='width: 50px;' value=' OK '>
	</form>

<?php } ?>

<br><br>

<!-- Set focus on 1st field... -->
<script type="text/javascript"><!--
document.frmNewFile.filename.focus();
//-->
</script>
