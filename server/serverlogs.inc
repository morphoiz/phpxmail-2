<?php

$msg = '';

// check if form is submitted
if (!empty($_GET)) {
	if (isset($_GET['do'])){
/*		$LogToUse = $_GET['ltype'] .'-' .$_GET['log'];
		if ($_GET['do'] == "delete") {
			if ($mail_server->cfgfileclear('logs/'.$LogToUse)) {
				$msg = "<img src=gfx/ico_check.gif> Log <b>".$LogToUse."</b> emptied successfully.";
			} else {
				$msg = '<img src=gfx/ico_warn.gif> Error clearing log: ' . $mail_server->xm_err_msg;
			}
			echo $msg ."<hr>";
			die;
		}
*/
	} else {
		// Make a report on all file types found in LOG subdirectory (so we become Xmail version indipendent)
		$LogFiles = $mail_server->filelist('logs', '*-*');
		if (count($LogFiles) == 0) {
			echo '<img src=gfx/ico_warn.gif>&nbsp;Does not exists any log file!<hr>';
			die;
		} else {
			$FileTypes = array();
			foreach ($LogFiles as $SingleFile) {
				$prefix = split('-', $SingleFile[0]);
				if (!array_key_exists($prefix[0],$FileTypes)){
					$FileTypes[$prefix[0]] = array ('type'=>$prefix[0], 'count'=>0, 'last'=>'', 'first'=>date("YmdHi"), 'size'=>0);
				}
				$FileTypes[$prefix[0]]['count']++;
				$FileTypes[$prefix[0]]['size'] += $SingleFile[1];
				if ($prefix[1] > $FileTypes[$prefix[0]]['last']) $FileTypes[$prefix[0]]['last'] = $prefix[1];
				if ($prefix[1] < $FileTypes[$prefix[0]]['first']) $FileTypes[$prefix[0]]['first'] = $prefix[1];
			}
			sort($FileTypes);
			reset($FileTypes);
		}
	}
}
?>

<span class="caption">server log files</span><br>
<table border=0 class='tbl'>
	<tr>
        <th width="75">type</th>
        <th width="50">files</th>
        <th width="75">tot. size</th>
        <th width="140">last log rotation</th>
        <th width="140">first log rotation</th>
    </tr>
<?php 
	$TotCount = $TotSize = 0;
	$last = array();
	foreach ($FileTypes as $row){ ?>
	<tr>
		<td><a href="main.php?action=serverlog&ltype=<?php echo $row['type']; ?>&log=<?php echo $row['last']; ?>&do=view"><img src="gfx/ico_log.gif" alt="View files" border="0"> <?php echo $row['type'] ?></a></td>
		<td><?php $TotCount += $row['count']; echo $row['count'] ?></td>
		<td><?php $TotSize += $row['size']; echo humansize($row['size']) ?></td>
		<td><?php ereg("([0-9]{4})([0-9]{2})([0-9]{2})([0-9]{2})([0-9]{2})", $row['last'], $last); echo "$last[1]-$last[2]-$last[3] $last[4]:$last[5]"?></td>
		<td><?php ereg("([0-9]{4})([0-9]{2})([0-9]{2})([0-9]{2})([0-9]{2})", $row['first'], $last); echo "$last[1]-$last[2]-$last[3] $last[4]:$last[5]"?></td>
	</tr>
<?PHP } ?>
	<tr><td colspan="5"><hr></td>	
	<tr><td>Total:</td>	
		<td><?php echo $TotCount ?></td>
		<td><?php echo humansize($TotSize) ?></td>
	</tr>
</table>
<br><br>
<hr>
<img src=gfx/ico_info.gif align=absmiddle title=info>&nbsp;Less recent files may be accessed inside theirs rispective log types.
<br><br>
