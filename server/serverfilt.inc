<?PHP
function RecordsCount($f){
	if (is_array($f)){
		if (isset($f[0])){
			if (is_array($f[0])){
				return count($f);
			} else {
				if (trim($f[0]) != "") {
					return count($f);
				} else {
					return count($f)-1;
				}
			}
		}
	}
	return 0;
}
?>

<span class="caption">on-line filters</span><br>
<table border=0 class='tbl'>
        <th width="250">type</th>
        <th width="175">count</th>
                <tr>
                    <td><img src="gfx/ico_filter_on.gif">
                    <a href="main.php?action=serverfilters&ftype=pre">
                    <img src='gfx/ico_properties.gif' alt='Show properties' height='16' width='16' border='0'>
                    Pre filters</a></td>
                    <td><?php $file = $mail_server->cfgfileget('filters.pre-data.tab'); echo RecordsCount($file); ?></td>
                </tr>
                <tr>
                    <td><img src="gfx/ico_filter_on.gif">
                    <a href="main.php?action=serverfilters&ftype=post">
                    <img src='gfx/ico_properties.gif' alt='Show properties' height='16' width='16' border='0'>
                    Post filters</a></td>
                    <td><?php $file = $mail_server->cfgfileget('filters.post-data.tab'); echo RecordsCount($file); ?></td>
                </tr>
</table>

<p>&nbsp;</p>

<span class="caption">off-line filters</span><br>
<table border=0 class='tbl'>
        <th width="250">type</th>
        <th width="175">count</th>
                <tr>
                    <td><img src="gfx/ico_filter_off.gif">
                    <a href="main.php?action=serverfilters&ftype=in">
                    <img src='gfx/ico_properties.gif' alt='Show properties' height='16' width='16' border='0'>
                    In filters</a></td>
                    <td><?php $file = $mail_server->cfgfileget('filters.in.tab'); echo RecordsCount($file); ?></td>
                </tr>
                <tr>
                    <td><img src="gfx/ico_filter_off.gif">
                    <a href="main.php?action=serverfilters&ftype=out">
                    <img src='gfx/ico_properties.gif' alt='Show properties' height='16' width='16' border='0'>
                    Out filters</a></td>
                    <td><?php $file = $mail_server->cfgfileget('filters.out.tab'); echo RecordsCount($file); ?></td>
                </tr>
</table>
<br>
<hr>
<br>