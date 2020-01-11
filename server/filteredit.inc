<?php

$msg = '';

$filter = urldecode($_REQUEST['filter']);

if (!isset($_POST['submit'])) {
	$filtercontent = '';
	$file = $mail_server->cfgfileget('filters/' .$filter);
	if (count($file) >> 1000){die("Filter file has more than 1000 entries\nRefusing to show it");}
	foreach ($file as $line){
		if (is_array($line) && $line != ''){
			$filtercontent .= implode("<tab>", str_replace('"', '', $line)) . "\n";
		} else {
			$filtercontent .= str_replace('"', '', $line) . "\n";
		}
	}
	if (substr($filtercontent, -1, 1) == "\n") $filtercontent = substr($filtercontent, 0, strlen($filtercontent) -1);
} else {
	$filtercontent = str_replace('"', "", trim($_REQUEST['FileEdit'])); // preserve for show it later
	if ($filtercontent != '') {
		// create a multiarray that will be passed to Xmail server
		$filtercontentA = explode ("\n",  $filtercontent);
		foreach ($filtercontentA as $key=>$row){
			$filtercontentA[$key] = str_replace("|", "<tab>", $row);
			$filtercontentA[$key] = explode ('<tab>', $filtercontentA[$key] );
			foreach ($filtercontentA[$key] as $key2=>$row2){
				$filtercontentA[$key][$key2] = '"' .$row2 .'"';
			}
		}
		//preset a 'saved successfull' message		
		$msg = "<img src=gfx/ico_check.gif> Values for <b>'" .$filter ."'</b> saved successfully";
	} else {
		//empty the array, so the file will be removed
		$filecontentA = array();
		//preset a 'deleted successfull' message		
		$msg = "<img src=gfx/ico_check.gif> <b>'".$filter ."'</b> was deleted successfully";
	}
	if (!$mail_server->cfgfileset('filters/' .$filter, $filtercontentA )) {
		$msg = '<img src=gfx/ico_warn.gif> Error saving values: ' . $mail_server->xm_err_msg;
	}
}
//re-read all...
$filtercontent = '';
$file = $mail_server->cfgfileget('filters/' .$filter);
if (count($file) >> 1000){die("Filter file has more than 1000 entries\nRefusing to show it");}
foreach ($file as $line){
	if (is_array($line) && $line != ''){
		$filtercontent .= implode("<tab>", str_replace('"', '', $line)) . "\n";
	} else {
		$filtercontent .= str_replace('"', '', $line) . "\n";
	}
}
if (substr($filtercontent, -1, 1) == "\n") $filtercontent = substr($filtercontent, 0, strlen($filtercontent) -1);

echo $msg;
?> 
<form name="frmFilterEdit" action="" method="post">
    <input type='hidden' name='action' value='filteredit'>
    <input type='hidden' name='filter' value='<?php echo $filter; ?>'>
    <table class='tbl'>
        <tr>
            <td colspan="2">
                <textarea name="FileEdit" rows="5" cols="60"><?php echo $filtercontent; ?></textarea></p></td></tr>
        <tr>
            <td> 
                <img src=gfx/ico_info.gif title="info">&nbsp;Use <b>&lt;tab&gt;</b> or vertical bar <B>|</B> for real tabs</td>
            <td align="right"><input type="submit" name="submit" value=" submit ">
    </table>
</form>
<hr>
<br>

<!-- Set focus on 1st field... -->
<script type="text/javascript"><!--
document.frmFilterEdit.FileEdit.focus();
//-->
</script>
