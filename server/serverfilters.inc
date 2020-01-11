<?php
                // module functions ---------------------------------------------------------------------
                //Create an HTML SELECT object with a selection of all filters files
                function FilterFilesSelection($objName, $opts){
                        //select user from a list
                        echo '<select name="' .$objName .'" ' .$opts .'style="width: 102pt;">';
                        $mail_server = $_SESSION['mail_server'];
                        foreach($mail_server->filelist('filters', '*.tab') as $filter){
                                        echo '<option';
                                        echo ' value="' .$filter[0] .'">'.$filter[0] .'</option>';
                        }
                        echo '</select>';
                }
                // end of module functions --------------------------------------------------------------


                $msg = '';
                $msg2 = '';
        $ftype = $_REQUEST['ftype'];
        switch ($ftype) {
                case 'pre':
                        $file = 'filters.pre-data.tab';
                        $type = 'on-line';
                        $msg2 .= 'The table above lists the filters that have to be applied, during a SMTP session, after the remote SMTP client sends the DATA command, and before XMail sends the response to the command.';
                        break;
                case 'post':
                        $file = 'filters.post-data.tab';
                        $type = 'on-line';
                        $msg2 .= 'The table above lists the filters that have to be applied, during a SMTP session, after XMail received the whole client DATA, and before XMail sends the final response to the DATA command (final messages ack).';
                        break;
                case 'in':
                        $file = 'filters.in.tab';
                        $type = 'off-line';
                        $msg2 .= 'The table above lists the filters that have to be applied to inbound messages (going to local mailboxes, may be also coming from a POP3 synchronization).';
                        break;
                case 'out':
                        $file = 'filters.out.tab';
                        $type = 'off-line';
                        $msg2 .= 'The table above lists the filters that have to be applied to outbound messages (delivered remotely).';
                        break;
        }

        //get the filter file
        $vars = $mail_server->cfgfileget($file);
        if (is_array($vars)){
              $fnum = (count($vars))+1;
        }else{
             echo "<img src=gfx/ico_warn.gif> Unable to find $file , please create it under MailRoot";
             die;
        }

        // check if form is submitted
        if (!empty($_POST)) {
        $data = array();
                //add/update existing filters
                for ($n = 1; $n < $fnum; $n++) {
                        $sen = $_POST[$n . "_sen"];
                        $rec = $_POST[$n . "_rec"];
                        $rem = $_POST[$n . "_rem"];
                        $loc = $_POST[$n . "_loc"];
                        $fil = $_POST[$n . "_fil"];
                        array_push($data, array("\"$sen\"","\"$rec\"","\"$rem\"","\"$loc\"","\"$fil\""));
                }
                //add new filter
                if (isset($_POST['xtr_sen']) && !isset($_POST['save'])){
                   if ($_POST['xtr_sen'] != "" || $_POST['xtr_rec'] != "" || $_POST['xtr_rem'] != "" || $_POST['xtr_loc'] != "" || $_POST['xtr_fil'] != ""){
                            //get fields
                            $xtr_sen = $_POST["xtr_sen"];
                            $xtr_rec = $_POST["xtr_rec"];
                            $xtr_rem = $_POST["xtr_rem"];
                            $xtr_loc = $_POST["xtr_loc"];
                            $xtr_fil = $_POST["xtr_fil"];
                            array_push($data, array("\"$xtr_sen\"","\"$xtr_rec\"","\"$xtr_rem\"","\"$xtr_loc\"","\"$xtr_fil\""));
                            $new = "<img src=gfx/ico_create.gif> A new filter was added.<br>";
                   }
                }
                //save filters
                if ($mail_server->cfgfileset($file, $data)) {
                       $msg = "<img src=gfx/ico_check.gif> Filters in $file were updated successfully <br>$new";
                } else {
                       $msg = "<img src=gfx/ico_warn.gif> Error saving filters in $file: " .$mail_server->xm_err_msg ."<br>";
                }
        }
        //delete filters
        if (isset($_GET['do']) && isset($_GET['line'])){
                if ($_GET['do'] == "delete"){
                        $line = $_GET['line'];
                        $i = 1;
                        foreach ($vars as $var) {
                                if ($i != $line){
                                    //add commas
                                    unset($d);
                                    foreach ($var as $v){
                                       $d[] = "\"$v\"";
                                    }
                                    $data[] = $d;
                                    //added
                                }
                        $i++;
                        }
                        if (is_array($data)){
                                  $write = $mail_server->cfgfileset($file, $data);
                        }else{
                                  $write = $mail_server->cfgfileclear($file, $data);
                        }
                        if ($write) {
                           $msg .= "<img src=gfx/ico_check.gif> Filter line $line was deleted successfully<br>";
                        } else {
                           $msg .= "<img src=gfx/ico_warn.gif> Error deleting filter line $line in $file: " . $mail_server->xm_err_msg ."<br>";
                        }
                }
        }

        //get the new filter file
        $vars = $mail_server->cfgfileget($file);
        if (is_array($vars)){
              $fnum = (count($vars))+1;
        }else{
             echo "<img src=gfx/ico_warn.gif> Unable to find $file , please create it under MailRoot";
             die;
        }

?>

<form name='frmServerFilt' action='main.php' method='post'>
        <input type='hidden' name='action' value='serverfilters'>
        <input type='hidden' name='ftype' value='<?php echo $ftype; ?>'>
        <input type='hidden' name='enabled' value=''>
        <?php echo $msg; ?>
        <table>
                <tr>
                        <td width="10">&nbsp;</td>
                        <td width="100"><b>Sender</b></td>
                        <td width="100"><b>Recipient</b></td>
                        <td width="100"><b>Remote Addr</b></td>
                        <td width="100"><b>Local Addr</b></td>
                        <td width="100"><b>Filename</b></td>
                </tr>
                <tr>
                        <td><a href='javascript:document.frmServerFilt.submit();'><img src=gfx/ico_create.gif border=0></a></td>
                        <td><input type='text' name='xtr_sen'></td>
                        <td><input type='text' name='xtr_rec'></td>
                        <td><input type='text' name='xtr_rem'></td>
                        <td><input type='text' name='xtr_loc'></td>
                        <td><?php FilterFilesSelection('xtr_fil', null); ?></td>
                </tr>
                <tr>
                        <td colspan="6">&nbsp;</td>
                </tr>
                <tr>
                <?php $i=0; foreach ($vars as $var) { $i++; if(is_array($var)){ ?>
                        <tr>
                               <td><a href=main.php?action=serverfilters&ftype=<?php echo $ftype; ?>&do=delete&line=<?php echo $i; ?>><img src=gfx/ico_delete.gif border=0></a></td>
                               <td><input type='text' name='<?php echo $i; ?>_sen' value='<?php echo($var[0]); ?>'></td>
                               <td><input type='text' name='<?php echo $i; ?>_rec' value='<?php echo($var[1]); ?>'></td>
                               <td><input type='text' name='<?php echo $i; ?>_rem' value='<?php echo($var[2]); ?>'></td>
                               <td><input type='text' name='<?php echo $i; ?>_loc' value='<?php echo($var[3]); ?>'></td>
                               <td><input type='text' name='<?php echo $i; ?>_fil' value='<?php echo($var[4]); ?>'></td>
                        </tr>
                <?php }} ?>
                <?php if(count($vars) > 0){ ?>
                 <tr>
                        <td colspan="6" align="right"><p>&nbsp;</p><input type="submit" name="save" value=" Save values "></td>
                </tr>
                <?php } ?>
        </table>
</form>
<hr>
<?php
        $msg = '<table><tr valign="top">';
                $msg .=        '<td><img src=gfx/ico_info.gif align=absmiddle title=info></td><td> the * character is a wildcard</td></tr>';
        $msg .= '<tr valign="top"><td><img src=gfx/ico_info.gif align=absmiddle title=info>&nbsp;</td><td>';
                $msg .= $msg2;
        $msg .= '</td></tr></table>';
        echo $msg;
?>
<hr>
<br><br>