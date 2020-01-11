<?PHP

// build an array of HTML codes for usage mapping...
$usagemap = array();
$temp_1=array();
$filters = array();
$filters[0] = 'filters.pre-data.tab';
$filters[1] = 'filters.post-data.tab';
$filters[2] = 'filters.in.tab';
$filters[3] = 'filters.out.tab';
$i=0;
foreach ($filters As $file){
	//get the filter file
	$vars = $mail_server->cfgfileget($file);
	//for each file fill the HTML code
	if (!$vars === false) {
		foreach ($vars as $key=>$nome){
			if (is_array($vars[0])){
				for ($t = 0; $t <= 3; $t++){
					if ($t != $i) {
						$temp_1[$t] = "<td align='center'>-</td>";
					} else {
						$temp_1[$t] = "<td align='center'><img src='gfx/ico_check.gif' height='16' width='16' border='0' valign='top'></td>";
					}
				}
				$usagemap[$vars[$key][4]] = implode ("",$temp_1);
			}
		}
	}
	$i++;
}
$files = $mail_server->filelist('filters', '*.tab');
foreach ($files as $filter){
	if (!key_exists($filter[0],$usagemap)){
		// this filter file is not used in neither pre-post-in-out filtering process
		$usagemap[$filter[0]] = "<td align='center'>-</td><td align='center'>-</td><td align='center'>-</td><td align='center'>-</td>";
	}
}
//...builded
?>

<span class="caption">server filters</span><br>
<table border=0 class='tbl'>
        <th >name</th>
        <th width="35">pre</th>
        <th width="35">post</th>
        <th width="35">in</th>
        <th width="35">out</th>
        <th >size</th>
<?php 
foreach ($files as $filter){
	echo '<tr>';
	echo 	'<td><a href="main.php?action=filterdel&filter=' .urlencode($filter[0]) .'"> <img src="gfx/ico_delete.gif" border="0"></a>&nbsp;';
	echo 	'<a href="main.php?action=filteredit&filter=' .urlencode($filter[0]) .'"><img src="gfx/ico_edit.gif"  border="0">&nbsp;' .$filter[0] .'</a></td>';
	echo	$usagemap[$filter[0]] .'<td>' .humansize($filter[1]) .'</td>' ;
	echo '</tr>';
}

?>
</table>
<br>
<hr>

<a href='main.php?action=filteradd'>
	<img src='gfx/ico_create.gif' alt='New filter' height='16' width='16' border='0' valign="top">
    <b>new filter file</b></a>
<br>