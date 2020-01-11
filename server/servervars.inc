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

$msg = '';

// check if form is submitted
if (!empty($_POST)) {
	if ((isset($_POST['save']) || isset($_POST['cmdAdd_x']))){
		$vars = array();
		$i=0;
		foreach($_POST as $var=>$val) {
			if (ereg("var_", $var)) {
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
			if ($_POST['xtr_var'] != '' && $_POST['xtr_value'] != ''){
				array_push($vars, array($_POST['xtr_var'], $_POST['xtr_value']));
			}
		}
		if ($mail_server->cfgfileset('server.tab', $vars)) {
			$msg = "<img src=gfx/ico_check.gif> Server config was saved successfully<hr>";
		} else {
			$msg = "<img src=gfx/ico_warn.gif> Error saving config: " . $mail_server->xm_err_msg ."<hr>";
		}
		unset($vars);
	}
}

$vars = array();

$file = $mail_server->cfgfileget('server.tab');
foreach ($file as $line) {
	if (ereg('#', $line[0])) {
		$line[0] = str_replace('"', '', substr($line[0], 1));
		$vars[$line[0]]['enabled'] = false;
	} else {
		$vars[$line[0]]['enabled'] = true;
	}
	$vars[$line[0]]['value'] = $line[1];
}

echo $msg;

?>

<script type='text/javascript'>
function enumEnabled() {
	tmp = '';
	e = document.frmServerVars.check;
	for (i = 0; i < e.length; i++) {
		tmp += e[i].value
	}
	document.frmServerVars.enabled.value = tmp;
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
}
</script>

<form name='frmServerVars' action='main.php' method='post' onsubmit='enumEnabled()'>
    <input type='hidden' name='action' value='servervars'>
    <input type='hidden' name='enabled' value=''>
    <table>
    	<tr><td>
    		<fieldset class="caption" style="width=100%"><legend><b>Add extra variable:</b>&nbsp;</legend>
    			<table>
		            <tr>
		                    <td>&nbsp;</td>
		                    <td><b>Server variable</b></td>
		                    <td><b>Value</b></td>
		            </tr>
		            <tr>
			                <td><input class="transparent" type="image" name="cmdAdd" alt="Add" value="Add" src=gfx/ico_create.gif></td>
		                    <td><input type='text' name='xtr_var' size='30' style='width: max;'></td>
		                    <td><input type='text' name='xtr_value' size='30' style='width: 400px;'></td>
		            </tr>
			        <tr><!--force alignement with the entire table-->
			                <td><input class="transparent" style='width: 16px; height: 6px'></td>
			                <td><input class="transparent" style='width: 200px; height: 6px'></td>
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
		            	if (is_array($var)){?>
		                    <tr>
		                        <td><?php echo PresetDynObject('R' .$i, $var['enabled'], $key); ?></td>
		                        <td><input type='text' name='var_<?php echo $key; ?>' size='30' style='width: 400px;' value='<?php echo $var['value']; ?>'></td>
		                    </tr>
		            <?php }} ?>
			        <tr><!--force alignement with the entire table-->
			                <td><input class="transparent" style='width: 16px; height: 6px'></td>
			                <td><input class="transparent" style='width: 200px; height: 6px'></td>
			                <td><input class="transparent" style='width: 400px; height: 6px'></td>
			        </tr>
			 	</table>
			 </fieldset>
			 <br>
		</td></tr>
		<tr>			                
                <td align="right">
                	<input type='submit' name='save' value=' Save values '>
                </td>
        </tr>
		</td></tr>
    </table>
</form>
<hr>
<img src=gfx/ico_info.gif align=absmiddle title=info>&nbsp;Clicking on <img src=gfx/ico_on.gif align=absmiddle> will disable the variable, clicking on <img src=gfx/ico_off.gif align=absmiddle> will enable it.<br>
<img src=gfx/ico_info.gif align=absmiddle title=info>&nbsp;To definitely delete a variable, simply empty it.
<br><br>
