<?php

        if ($mail_server->xm_ctrl_t != "s_admin"){
                include('denied.inc');
                die;
        }

        // set vars
        $msg = '';

        // check if form is submitted
        if (isset($_POST['submit'])) {

                //make sure file is writable if not chmod it
                do_chmod("config.php");

                foreach ($_POST as $name => $value){
                        if (strstr($name, 'cfg_') != ''){
                        write_conf("config.php","$name='${$name}'","$name='$value'");
                        ${$name} = $value;
                        }
                }

                $msg = "<img src=gfx/ico_check.gif> Configuration was updated successfully.<br>";
        }

?>

<form name='frmNewServer' action='' method='post'>
        <input type='hidden' name='action' value='config'>
        <?php echo $msg; ?>
        <table>
        <th width='250' align="left">option</th>
        <th width='150' align="left">status</th><br>
                <tr>
                        <td><b>Show stats in userlist:<b><td>
                        <select name='cfg_userlist_stat'>
                        <option value=on <?if($cfg_userlist_stat == "on"){echo "selected";}?> >On</option>
                        <option value=off <?if($cfg_userlist_stat == "off"){echo "selected";}?> >Off</option>
                        </select>
                </tr>
                <tr>
                        <td><b>Show stats in uservars:</b><td>
                        <select name='cfg_uservars_stat'>
                        <option value=on <?if($cfg_uservars_stat == "on"){echo "selected";}?> >On</option>
                        <option value=off <?if($cfg_uservars_stat == "off"){echo "selected";}?> >Off</option>
                        </select>
                </tr>
                <tr>
                        <td><b>Enable code debugging:</b><td>
                        <select name='cfg_debug_stat'>
                        <option value=on <?if($cfg_debug_stat == "on"){echo "selected";}?> >On</option>
                        <option value=off <?if($cfg_debug_stat == "off"){echo "selected";}?> >Off</option>
                        </select>
                </tr>
                <tr>
                        <td><b>Self checking for updates:</b><td>
                        <select name='cfg_update_chk'>
                        <option value=on <?if($cfg_update_chk == "on"){echo "selected";}?> >On</option>
                        <option value=off <?if($cfg_update_chk == "off"){echo "selected";}?> >Off</option>
                        </select>
                </tr>
                <tr>
                        <td><b>Allow visitors to register a mail account:</b><td>
                        <select name='cfg_visitor_reg'>
                        <option value=on <?if($cfg_visitor_reg == "on"){echo "selected";}?> >On</option>
                        <option value=off <?if($cfg_visitor_reg == "off"){echo "selected";}?> >Off</option>
                        </select>
                </tr>
                <tr>
                        <td><b>Allow 'Admin' info in 'Login' start page:</b><td>
                        <select name='cfg_allow_admin_info'>
                        <option value=on <?if($cfg_allow_admin_info == "on"){echo "selected";}?> >On</option>
                        <option value=off <?if($cfg_allow_admin_info == "off"){echo "selected";}?> >Off</option>
                        </select>
                </tr>
                <tr>
                        <td><b>Allow cross domain aliases to be created:</b><td>
                        <select name='cfg_allow_cross_alias'>
                        <option value=on <?if($cfg_allow_cross_alias == "on"){echo "selected";}?> >On</option>
                        <option value=off <?if($cfg_allow_cross_alias == "off"){echo "selected";}?> >Off</option>
                        </select>
                </tr>                
                <tr>
                        <td><b>Server information filename:*</b>
                        <td><input type='text' name='cfg_servers' size='30' value='<?php echo $cfg_servers; ?>'>
                </tr>
                <tr>
                        <td colspan="2" align="right"><input type='submit' name='submit' value=' submit '>
                </tr>
        </table>
</form>
<hr>
<img src=gfx/ico_info.gif align=absmiddle title=info> * Upon changing the servers filename also modify the .htaccess file under the phpxmail dir accordingly
<br><br>