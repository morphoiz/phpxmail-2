<?php


set_time_limit(240);
$frozmsg = explode(":", $_REQUEST['sel_frozmsg']);
$msg = "Frozen message <b>".$frozmsg[0].", ".$frozmsg[1].", ".$frozmsg[2];
$themessage = '';

$HTMLtagsColor = 'Navy';
foreach($mail_server->frozgetmsg($frozmsg[0], $frozmsg[1], $frozmsg[2]) as $frozmsgline) {
	if (is_array($frozmsgline)) {
		foreach($frozmsgline as $frozmsglineitem) {
			$frozmsglineitem = str_replace('<', '&lt;', $frozmsglineitem);
			$frozmsglineitem = str_replace('>', '&gt;', $frozmsglineitem);
			if (trim($frozmsglineitem) != '') $themessage .= '&nbsp;&nbsp;&nbsp;'.$frozmsglineitem .'<br>';
		}
	} else {
		if ($frozmsgline == '' && $HTMLtagsColor != 'Gray' ) {
			$themessage .= '<br><font color="#FF9900">mail body:</font>';
			$HTMLtagsColor = 'Gray';
		}
		$frozmsgline = str_replace('<<MAIL-DATA>>', '--MAIL-DATA--', $frozmsgline);
		$frozmsgline = str_replace('<', '&lt;<font color="' .$HTMLtagsColor .'">', $frozmsgline);
		$frozmsgline = str_replace('>', '&gt;</font>', $frozmsgline);
		$frozmsgline = str_replace('--MAIL-DATA--', '<br><font color="#FF9900">message header:</font>', $frozmsgline);
		$themessage .= "$frozmsgline<br>";
	}
}

?>

<p><?php echo $msg; ?></p>
<font color="#FF9900">Xmail server:</font><br>

<?php        echo $themessage; ?>
<hr>

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