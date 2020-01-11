<?php

function PresetDynObject($Dyn_Item, $status, $varname) {
	$block = "<a href='javascript:toggle_OnOff(\"" .$Dyn_Item ."\");'>";
	if ($status){
		$block .= "\n\r<img id='" .$Dyn_Item ."_image' name='" .$Dyn_Item ."_image' src='gfx/ico_on.gif' height='16px' width='16px' align='absmiddle' border='0' alt='Disable variable'></a> ";
		$block .= "\n\r<input type='hidden' id='" .$Dyn_Item ."_status' name='check' value='1'></td>";
		$block .= "\n\r<td id='" .$Dyn_Item ."_text' style='color: black;'><b>" .$varname ."</b>";
	} else {
		$block .= "\n\r<img id='" .$Dyn_Item ."_image' name='" .$Dyn_Item ."_image' src='gfx/ico_off.gif' height='16px' width='16px' align='absmiddle' border='0' alt='Enable variable'></a> ";
		$block .= "\n\r<input type='hidden' id='" .$Dyn_Item ."_status' name='check' value='0'></td>";
		$block .= "\n\r<td id='" .$Dyn_Item ."_text' style='color: gray;'><b>" .$varname ."</b>";
	}
	return $block;
}

// preset variables for use this page in GLOBAL Mode or on DOMAIN basis...
if (!isset($domain) || ($domain == '')) {
	// GLOBAL...
	$UserDefUsed = true;
	$FileToUse = 'userdef.tab';
} else {
	// Per DOMAIN basis...
	$UserDefUsed = count($mail_server->filelist('domains/' .$domain, 'userdef.tab'));
	$FileToUse = 'domains/' .$domain .'/userdef.tab';
}
$msg = '';

// check if form is submitted
if (!empty($_POST)) {
	$domain = $_POST['domain'];
	if (isset($_POST['yes'])) {
		$UserDefUsed = true;
		$file = $mail_server->cfgfileget('userdef.tab');
		if ($mail_server->cfgfileset('domains/' .$domain .'/userdef.tab', $file)) {
			$msg = "<img src=gfx/ico_check.gif>&nbsp;User defaults values copied successfully<hr>";
		} else {
			$msg = "<img src=gfx/ico_warn.gif>&nbsp;Error saving values: " . $mail_server->xm_err_msg ."<hr>";
		}
	}
	if (isset($_POST['no'])) {
		echo "<img src=gfx/ico_stop.gif>&nbsp;Indipendent user defaults template for '" .$domain ."' was <b>NOT</b> created.";
		die;
	}
	if ((isset($_POST['save']) || isset($_POST['cmdAdd_x'])) && $UserDefUsed){
		$vars = array();
		$i=0;
		foreach($_POST as $var=>$val) {
			if (preg_match("var_", $var)) {
				if ($val != '') {
					$var = substr($var, 4);
					if (substr($_POST['enabled'], $i, 1) == 0) {
						array_push($vars, array("#\"$var\"", "\"$val\""));
					} else {
						array_push($vars, array("\"$var\"", "\"$val\""));
					}
				}
				$i++;
			}
		}

		// check if submitted from PLUS (add) icons
		if (isset($_POST['cmdAdd_x'])){
			if ($_POST['xtr_var'] != '' && $_POST['xtr_value'] != '') {
				array_push($vars, array($_POST['xtr_var'], $_POST['xtr_value']));
			}
		}
		if ($mail_server->cfgfileset($FileToUse, $vars)) {
			$msg = "<img src=gfx/ico_check.gif>&nbsp;User defaults values in 'userdef.tab' saved successfully<hr>";
		} else {
			$msg = "<img src=gfx/ico_warn.gif>&nbsp;Error saving values: " . $mail_server->xm_err_msg ."<hr>";
		}
		unset($vars);
	}
	if (isset($_POST['remove']) && $UserDefUsed){
		$dummy = array();
		if ($mail_server->cfgfileset($FileToUse, $dummy)) {
			$UserDefUsed = false;
			$msg = "<img src=gfx/ico_check.gif>&nbsp;User defaults template removed successfully<hr>";
		} else {
			$msg = "<img src=gfx/ico_warn.gif>&nbsp;Error removing template: " . $mail_server->xm_err_msg ."<hr>";
		}
	}
}

if ($UserDefUsed) {
	// populate a list of all available user vars (xmail version 1.21)
	$vars = array();
	$vars['MaxMBSize'] = array('var'=>'MaxMBSize', 'enabled'=>false, 'value'=>'10000');
	$vars['MaxMessageSize'] = array('var'=>'MaxMessageSize', 'enabled'=>false, 'value'=>'0');
	$vars['SmtpPerms'] = array('var'=>'SmtpPerms', 'enabled'=>false, 'value'=>'MR');
	$vars['DisableEmitAuthUser'] = array('var'=>'DisableEmitAuthUser', 'enabled'=>false, 'value'=>'1');
	$vars['ReceiveEnable'] = array('var'=>'ReceiveEnable', 'enabled'=>false, 'value'=>'1');
	$vars['PopEnable'] = array('var'=>'PopEnable', 'enabled'=>false, 'value'=>'1');
	$vars['RealName'] = array('var'=>'RealName', 'enabled'=>false, 'value'=>'??');
	$vars['HomePage'] = array('var'=>'HomePage', 'enabled'=>false, 'value'=>'??');
	$vars['Address'] = array('var'=>'Address', 'enabled'=>false, 'value'=>'??');
	$vars['Telephone'] = array('var'=>'Telephone', 'enabled'=>false, 'value'=>'??');

	// add also those for mailing list vars
	$vars['UseReplyTo'] = array('var'=>'UseReplyTo', 'enabled'=>false, 'value'=>'1', 'mlist'=>true);
	$vars['ClosedML'] = array('var'=>'ClosedML', 'enabled'=>false, 'value'=>'1', 'mlist'=>true);
	$vars['ListSender'] = array('var'=>'ListSender', 'enabled'=>false, 'value'=>'??', 'mlist'=>true);

	// now enable those vars stored in the file and that are uncommented
	$file = $mail_server->cfgfileget($FileToUse);
	foreach ($file as $line) {
		if (preg_match('#', $line[0])) {
			$line[0] = str_replace('"', '', substr($line[0], 1));
			$vars[$line[0]]['enabled'] = false;
		} else {
			$vars[$line[0]]['enabled'] = true;
		}
		$vars[$line[0]]['value'] = $line[1];
	}
}
echo $msg;

if ($UserDefUsed) {
?>

<script type='text/javascript'>
function enumEnabled() {
	tmp = '';
	e = document.frmUserDefaults.check;
	for (i = 0; i < e.length; i++) {
		tmp += e[i].value
	}
	document.frmUserDefaults.enabled.value = tmp;
	document.frmUserDefaults.save.focus();
	return false;
}

function toggle_OnOff(id) {
	if (document.getElementById(id + '_status')) {
		var dom = document.getElementById(id + '_status').value;
		if (dom == '0') {
			document.getElementById(id + '_status').value = '1';
			document.getElementById(id + '_image').src = 'gfx/ico_on.gif';
			document.getElementById(id + '_image').alt = 'Disable variable'
			document.getElementById(id + '_text').style.color = 'black';
		} else if (dom == '1') {
			document.getElementById(id + '_status').value = '0';
			document.getElementById(id + '_image').src = 'gfx/ico_off.gif';
			document.getElementById(id + '_image').alt = 'Enable variable'
			document.getElementById(id + '_text').style.color = 'gray';
		}
	}
	document.frmUserDefaults.save.focus();
}
</script>

<form name='frmUserDefaults' action='main.php' method='post' onsubmit='enumEnabled()'>
    <input type='hidden' name='action' value='<?php echo $action; ?>'>
    <input type='hidden' name='enabled' value=''>
    <?php if ($domain !='') echo "<input type='hidden' name='domain' value='" .$domain ."'>"; ?>
    <table>
    	<tr><td>
    		<fieldset class="caption" style="width=100%"><legend><b>Add custom variable:</b>&nbsp;</legend>
    			<table>
			        <tr>
			                <td>&nbsp;</td>
			                <td><b>Variable</b></td>
			                <td><b>Value</b></td>
			        </tr>
			        <tr>
			                <td><input class="transparent" type="image" name="cmdAdd" alt="Add" value="Add" src=gfx/ico_create.gif></td>
			                <td><input type='text' name='xtr_var' size='30' style='width: 150px;'></td>
			                <td><input type='text' name='xtr_value' size='30' style='width: 400px;'></td>
			        </tr>
			        <tr><!--force alignement with the entire table-->
			                <td><input class="transparent" style='width: 16px; height: 6px'></td>
			                <td><input class="transparent" style='width: 150px; height: 6px'></td>
			                <td><input class="transparent" style='width: 400px; height: 6px'></td>
			        </tr>
			 	</table>
			 </fieldset>
			 <br>
		</td></tr>
    	<tr><td>
    		<fieldset class="caption" style="width=100%"><legend><b>User:</b>&nbsp;</legend>
    			<table>
					<?php 
					$i=0;
					foreach ($vars as $key=>$var) {
						$i++;
			        	if (!isset($var['mlist'])){ ?>
			                <tr>
		                        <td><?php echo PresetDynObject('R' .$i, $var['enabled'], $key); ?></td>
		                        <td><input type='text' name='var_<?php echo $key; ?>' size='30' style='width: 400px;' value='<?php echo $var['value']; ?>'></td>
			                </tr>
			        <?php }} ?>
			        <tr><!--force alignement with the entire table-->
			                <td><input class="transparent" style='width: 16px; height: 6px'></td>
			                <td><input class="transparent" style='width: 150px; height: 6px'></td>
			                <td><input class="transparent" style='width: 400px; height: 6px'></td>
			        </tr>
			 	</table>
			 </fieldset>
			 <br>
		</td></tr>
    	<tr><td>
    		<fieldset class="caption" style="width=100%"><legend><b>Mailing list:</b>&nbsp;</legend>
    			<table>
					<?php foreach ($vars as $key=>$var) {
						$i++;
			        	if (isset($var['mlist'])){?>
			                <tr>
		                        <td><?php echo PresetDynObject('R' .$i, $var['enabled'], $key); ?></td>
		                        <td><input type='text' name='var_<?php echo $key; ?>' size='30' style='width: 400px;' value='<?php echo $var['value']; ?>'></td>
			                </tr>
			        <?php }} ?>
			        <tr><!--force alignement with the entire table-->
			                <td><input class="transparent" style='width: 16px; height: 6px'></td>
			                <td><input class="transparent" style='width: 150px; height: 6px'></td>
			                <td><input class="transparent" style='width: 400px; height: 6px'></td>
			        </tr>
			 	</table>
			 </fieldset>
			 <br>
		</td></tr>
		<tr>			                
                <td align="right">
                	<?php if ($domain !='') echo "<input type='submit' name='remove' value=' Remove template '>&nbsp;"; ?>
                	<input type='submit' name='save' value=' Save values '>
                </td>
        </tr>
		</td></tr>
    </table>
</form>
<hr>
<img src=gfx/ico_info.gif align=absmiddle title=info>&nbsp;Clicking on <img src=gfx/ico_on.gif align=absmiddle> will disable the variable, clicking on <img src=gfx/ico_off.gif align=absmiddle> will enable it.<br>
<img src=gfx/ico_info.gif align=absmiddle title=info>&nbsp;To delete a variable simply empty it (this method is applicable only to custom variables)
<br><br>
<!-- Set focus on 1st field... -->
<script type="text/javascript"><!--
document.frmUserDefaults.save.focus();
//-->
</script>
<?php } else { ?>
	<table>
		<tr valign="top">
			<td><img src=gfx/ico_notice.gif>&nbsp;</td>
			<td>This domain does not have its own user defaults template (it is referring to the server global defaults).<br><br>
			Do you want to create an indipendent user defaults template that it will be valid only for this domain?</td>
		</tr>
	</table>
	<br><br>
	<form name='frmCopyDefs' action='main.php' method='post'>
		<input type='hidden' name='action' value='<?php echo $action; ?>'>
		<input type='hidden' name='domain' value='<?php echo $domain; ?>'>
		<input type='submit' name='yes' style='width: 50px;' value=' Yes '>
		<input type='submit' name='no' style='width: 50px;' value=' No '>
	</form>
<?php } ?>