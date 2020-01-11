<?php


  $frozmsg = explode(":", $_REQUEST['sel_frozmsg']);

  $msg = "Logfile for frozen message <b>".$frozmsg[0].", ".$frozmsg[1].", ".$frozmsg[2];

?>

<p><?php echo $msg; ?></p>

<?php foreach ($mail_server->frozgetlog($frozmsg[0], $frozmsg[1], $frozmsg[2]) as $frozlogline) {
        echo "$frozlogline<br>";
} ?>

<form action='main.php' method='get'>
        <input type='hidden' name='action' value='frozsubmit'>
        <input type='hidden' name='sel_frozmsg' value='<?php echo "$frozmsg[0]:$frozmsg[1]:$frozmsg[2]"; ?>'>
        <input type='submit' name='click' style='width: 50px;' value='Submit'>
</form>

<form action='main.php' method='get'>
        <input type='hidden' name='action' value='frozdel'>
        <input type='hidden' name='sel_frozmsg' value='<?php echo "$frozmsg[0]:$frozmsg[1]:$frozmsg[2]"; ?>'>
        <input type='submit' name='click' style='width: 50px;' value='Delete'>
</form>