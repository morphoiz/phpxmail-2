<?php
	

	if (session_destroy()) {
		$msg = "<img src=gfx/ico_check.gif> You are logged out...";
		echo "<script>parent.menu.location.reload();</script>";
	} else {
		$msg = "<img src=gfx/ico_warn.gif> Error logging out";
	}

?>

<?php echo $msg; ?>

<br><br>
