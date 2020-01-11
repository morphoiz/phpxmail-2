<?PHP

$msg='';

// check if form is submitted
if (!empty($_POST)) {
	if (isset($_POST['psync'])){
//		$cmd[] = 'Psync \t now';
		if ($mail_server->startpsync()){
			$msg = "<img src=gfx/ico_check.gif> Synchronization started successfully.";
		} else {
			$msg = '<img src=gfx/ico_warn.gif> Error executing sync. command: ' . $mail_server->xm_err_msg;
		}
	}
}

echo $msg .'</p>';
?>

<form action='' method='post'>
        <input type='hidden' name='action' value='servercmd'>
        <input type='submit' name='psync' style='width: 150px;' value='Start a POP3 sync.'>
</form>

