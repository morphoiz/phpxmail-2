<?php

        set_time_limit(240);

        $msg = '';
        $ltgt = array("<", ">");

        // check if form is submitted
        if (!empty($_POST)) {
                if ($_POST['do'] == "Delete Checked") {
                        $num = 0;
                        foreach($mail_server->frozlist() as $frozmsg) {
                                $expected = str_replace('.', '_', $frozmsg[0]).":".$frozmsg[1].":".$frozmsg[2];
                                if ($_POST[$expected] == 'on'){
                                          if ($mail_server->frozdel($frozmsg[0], $frozmsg[1], $frozmsg[2])) {
                                                $num += 1;
                                          } else { echo $mail_server->xm_err_msg."<br>"; }
                                }
                        }
                        $msg = "<img src=gfx/ico_check.gif>Deleted <b>$num</b> frozen messages from the xmail spool.<br>";
                }
                if ($_POST['do'] == "Resubmit Checked") {
                        $num = 0;
                        foreach($mail_server->frozlist() as $frozmsg) {
                                $expected = str_replace('.', '_', $frozmsg[0]).":".$frozmsg[1].":".$frozmsg[2];
                                if ($_POST[$expected] == 'on'){
                                          if ($mail_server->frozsubmit($frozmsg[0], $frozmsg[1], $frozmsg[2])) {
                                                $num += 1;
                                          } else { echo $mail_server->xm_err_msg."<br>"; }
                                }
                        }
                        $msg = "<img src=gfx/ico_check.gif>Resubmitted <b>$num</b> frozen messages from the xmail spool.<br>";
                }
        }

?>
<script language="JavaScript">
function selectAll(f,n,v) {
        var chk   = ( v == null ? true : v );
        for (i = 0; i < f.elements.length; i++) {
                if( f.elements[i].type == 'checkbox' ) {
                        f.elements[i].checked = chk;
                }
        }
}
function formSub() {
         document.frmFrozen.submit();
}
</script>

<form name='frmFrozen' action='main.php' method='post'>
<input type='hidden' name='action' value='frozlist'>
<?php echo $msg; ?>
<span class="caption">frozen messages</span><br>
<table border=0 class='tbl'>
        <th width="5"><input type=checkbox onClick='selectAll(this.form)'></th>
        <th width="75">actions</th>
        <th width="25">name</th>
        <th width="17">sender</th>
        <th width="17">recipient</th>
        <th width="5">date</th>
        <th width="5">size</th>
<?php        foreach($mail_server->frozlist() as $frozmsg) { ?>
        <tr valign="baseline">
                <td>
                        <input type=checkbox name=<?php echo "$frozmsg[0]:$frozmsg[1]:$frozmsg[2]"; ?> >
                        <td><a href="main.php?action=frozdel&sel_frozmsg=<?php echo "$frozmsg[0]:$frozmsg[1]:$frozmsg[2]"; ?>">
                                <img src='gfx/ico_delete.gif' alt='Delete frozen msg' height='16' width='16' border='0'></a>
                        <a href="main.php?action=frozsubmit&sel_frozmsg=<?php echo "$frozmsg[0]:$frozmsg[1]:$frozmsg[2]"; ?>">
                                <img src='gfx/ico_re.gif' alt='Resubmit frozen msg' border='0'></a>
                        <a href="main.php?action=frozgetlog&sel_frozmsg=<?php echo "$frozmsg[0]:$frozmsg[1]:$frozmsg[2]"; ?>">
                                <img src='gfx/ico_log.gif' alt='get frozen msg log' height='16' width='16' border='0'></a>
                                <a href="main.php?action=frozgetmsg&sel_frozmsg=<?php echo "$frozmsg[0]:$frozmsg[1]:$frozmsg[2]"; ?>">
                                        <img src='gfx/ico_see.gif' alt='get frozen msg' height='16' width='16' border='0'></a>
                        <td><font size=-2><?php echo $frozmsg[1]."/".$frozmsg[2]."/froz/".limitch($frozmsg[0],10); ?></font>
                        <td><font size=-2><?php echo htmlentities(str_replace($ltgt, ' ', $frozmsg[3])); ?></font>
                        <td><font size=-2><?php echo htmlentities(str_replace($ltgt, ' ', $frozmsg[4])); ?></font>
                        <td><font size=-2><?php echo $frozmsg[5]; ?></font>
                        <td><font size=-2><?php echo humansize($frozmsg[6]); ?></font>

        </tr>
<?php } ?>
</table>
<br>
<hr>
<br>
<input type='submit' name='do' value='Delete Checked'>
&nbsp;
<input type='submit' name='do' value='Resubmit Checked'>
</form>