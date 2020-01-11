<?php

$msg = '';
if (ini_get('allow_url_fopen') == 0){
	$msg .= "<img src=gfx/ico_warn.gif> This server does not allow accesing of remote files from php.<br>";
}else{
	$available = file ('http://phpxmail.sourceforge.net/version.txt');
}
$installed = $cfg_version_num;
if (!$available){
	$msg .= "<img src=gfx/ico_warn.gif> Coud not get new version number";
	$disabled = "disabled";
} else {
	if ($installed >= rtrim($available[0])) {
		$msg .= "<img src=gfx/ico_stop.gif> Installed version of PHPXmail is the latest one, you don't need to update.";
		$disabled = "disabled";
	} else {
		$msg .= "<img src=gfx/ico_check.gif> New version of PHPXmail is available!";
		$disabled = "";
	}
}

?>

<form name='frmUpdate' action='http://phpxmail.sourceforge.net' method='post'>
        <?php echo $msg; ?>
        <table border='0' class='tbl'>
                <tr>
                        <td><b>Installed version:<b></td>
                        <td><?php echo $installed ?></td>
                <tr>
                        <td><b>Latest version:</b></td>
                        <td><?php echo $available[0] ?></td>
                <tr>
                        <td colspan="2" align="right"><input type='submit' name='submit' value='Get new version !' <?php echo $disabled ?> >
        </table>
</form>
<hr>
<br><br>
