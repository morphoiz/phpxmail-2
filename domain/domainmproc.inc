<?php

$msg = '';

//$filename = $_REQUEST['username'];
$filename = 'mailproc.tab';

if (!isset($_POST['submit'])) {
	$filecontent = '';
	$data = $mail_server->cfgfileget('domains/' .$domain .'/' .$filename);
	if (is_array($data)) {
		foreach ($data as $line){
			if (is_array($line) && $line != ''){
				$filecontent .= implode("<tab>", str_replace('"', '', $line)) . "\n";
			} else {
				$filecontent .= str_replace('"', '', $line) . "\n";
			}
		}
	}
	if (substr($filecontent, -1, 1) == "\n") $filecontent = substr($filecontent, 0, strlen($filecontent) -1);
} else {
	$filecontent = trim(str_replace('"', '', $_REQUEST['data'])); // preserve for show it later
	if ($filecontent != '') {
		// create a multiarray that will be passed to Xmail server
		$filecontentA = explode ("\n",  str_replace("\r", '', $filecontent));
		foreach ($filecontentA as $key=>$row){
			$filecontentA[$key] = str_replace("|", "<tab>", $row);
			$filecontentA[$key] = explode ('<tab>', $filecontentA[$key] );
			foreach ($filecontentA[$key] as $key2=>$row2){
				$filecontentA[$key][$key2] = '"' .$row2 .'"';
			}
		}
		//preset a 'saved successfull' message		
		$msg = "<img src=gfx/ico_check.gif> Values for <b>'" .$filename ."'</b> saved successfully";
	} else {
		$filecontentA = array();		//empty the array, so the file will be removed
		$msg = "<img src=gfx/ico_check.gif> <b>'".$filename ."'</b> was deleted successfully";
	}
	if (!$mail_server->cfgfileset('domains/' .$domain .'/' .$filename, $filecontentA )) {
		$msg = '<img src=gfx/ico_warn.gif> Error saving values: ' . $mail_server->xm_err_msg;
	}
}

//re-read all...
$filecontent = '';
$data = $mail_server->cfgfileget('domains/' .$domain .'/' .$filename);
if (is_array($data)) {
	foreach ($data as $line){
		if (is_array($line) && $line != ''){
			$filecontent .= implode("<tab>", str_replace('"', '', $line)) . "\n";
		} else {
			$filecontent .= str_replace('"', '', $line) . "\n";
		}
	}
}
if (substr($filecontent, -1, 1) == "\n") $filecontent = substr($filecontent, 0, strlen($filecontent) -1);

echo $msg;

?> 
<form name="frmFileEdit" action="" method="post">
    <input type='hidden' name='action' value='domainmproc'>
    <table class='tbl'>
        <tr>
            <td colspan="2">
                <textarea name="data" rows="5" cols="50"><?php echo $filecontent; ?></textarea></p></td></tr>
        <tr>
            <td> 
                <img src=gfx/ico_info.gif title="info">&nbsp;Use <b>&lt;tab&gt;</b> or vertical bar <B>|</B> for real tabs</td>
            <td align="right"><input type="submit" name="submit" value=" submit ">
        </tr>
        <tr>
            <td> 
                <img src=gfx/ico_info.gif title="info">&nbsp;It is not required to delimit fields with the <b>"</b> characters</td>
        </tr>
    </table>
</form>
<hr>
<br>
<!-- Set focus on 1st field... -->
<script type="text/javascript"><!--
document.frmFileEdit.data.focus();
//-->
</script>
