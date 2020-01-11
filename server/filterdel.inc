<?php
$msg = '';
$filter = urldecode($_GET['filter']);

// check if form is submitted
if (!empty($_POST)) {
	$filter = $_POST['filter'];
	if (isset($_POST['yes'])) {
		$dummy = array();
		if ($mail_server->cfgfileset('filters/' .$filter, $dummy)) {
			$msg = "<img src=gfx/ico_check.gif> Filter <b>'".$filter ."'</b> delete successfully.";
		} else {
			$msg = '<img src=gfx/ico_warn.gif> Error deleting filter: ' . $mail_server->xm_err_msg;
		}
	} else {
		$msg = "<img src=gfx/ico_stop.gif> Filter <b>'".$filter ."'</b> is <b>NOT</b> deleted";
	}
}

if (empty($_POST)) { 
?>

	<p>You are about to remove filter <b><?php echo "'".$filter ."'"; ?></b></p>
	<p>Do you really want to remove this filter?</p>

	<form name='frmDelfilter' action='main.php' method='post'>
		<input type='hidden' name='action' value='filterdel'>
		<input type='hidden' name='filter' value='<?php echo $filter; ?>'>
		<input type='submit' name='yes' style='width: 50px;' value=' Yes '>
		<input type='submit' name='no' style='width: 50px;' value=' No '>
	</form>

<?php } else { ?>

	<p><?php echo $msg; ?></p>

	<form action='main.php' method='post'>
		<input type='hidden' name='action' value='filterlist'>
		<input type='submit' name='click' style='width: 50px;' value=' OK '>
	</form>

<?php } ?>
				
