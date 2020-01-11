<?php

        $msg = '';

             $advanced = '';
             $l_domains = array();
             $p_domains = array();

        // check if form is submitted
        if (isset($_POST['submit'])) {
            $advanced = $_POST['advanced'];
            $l_redirects = $_POST['l_redirects'];
            $r_redirects = $_POST['r_redirects'];
                // parse info
                $l_domains = explode(',', $l_redirects);
                $p_domains = explode(',', $r_redirects);
                $vars = str_replace('<tab>', "\t", $advanced);
                foreach ($l_domains as $l_domain)
                        $vars .= "lredirect\t$l_domain\n";
                foreach ($p_domains as $p_domain)
                        $vars .= "redirect\t$p_domain\n";
                // save custom domain
                if ($mail_server->custdomset($domain, $vars)) {
                        $msg = "<img src=gfx/ico_check.gif> Custom domain <b>$domain</b> saved successfully.<br>";
                } else {
                        $msg = '<img src=gfx/ico_warn.gif> Error saving custom domain: ' . $mail_server->xm_err_msg;
                }
        } else {
                // get custom domain
                $proc = $mail_server->custdomget($domain);
                // parse info
                foreach($proc as $entry) {
                     if (is_array($entry)){
                        $n = sizeof($entry) - 1;
                        switch ($entry[0]) {
                                case 'lredirect':
                                     for($i = 1; $i <= $n; $i++) {
                                        array_push($l_domains, $entry[$i]);
                                     }
                                     break;
                                case 'redirect':
                                     for($i = 1; $i <= $n; $i++) {
                                        array_push($p_domains, $entry[$i]);
                                     }
                                     break;
                                default:
                                     $advanced .= implode("<tab>", $entry) . "\n";
                        }
                     } else {
                        $advanced .= $entry . "\n";
                     }
                }
         }

?>

<script language="JavaScript">

        function AddOption (List, Text) {
                List.options[List.options.length] = new Option(Text, Text, false, false);
        }

        function DelOption (List) {
                for (var i=(List.options.length-1); i>=0; i--) {
                        var o = List.options[i];
                        if (o.selected) List.options[i] = null;
                }
        }

        function SaveIt() {
                l = frmCustDomain.l_domains.options;
                r = frmCustDomain.p_domains.options;
                a = frmCustDomain.advanced.value;
                frmCustDomain.l_redirects.value = '';
                frmCustDomain.r_redirects.value = '';
                for (var i=0;i<l.length;i++) {
                        frmCustDomain.l_redirects.value += l[i].value + ',';
                }
                for (var i=0;i<r.length;i++) {
                        frmCustDomain.r_redirects.value += r[i].value + ',';
                }
                frmCustDomain.l_redirects.value = frmCustDomain.l_redirects.value.slice(0,-1);
                frmCustDomain.r_redirects.value = frmCustDomain.r_redirects.value.slice(0,-1);
                return true;
        }

</script>

<form name='frmCustDomain' action='main.php' method='post' onSubmit="return SaveIt();">
        <input type='hidden' name='action' value='custdomget'>
        <input type='hidden' name='domain' value='<?php echo $domain; ?>'>
        <input type='hidden' name='l_redirects' value=''>
        <input type='hidden' name='r_redirects' value=''>
        <?php echo $msg; ?>
        <table border='0' width='80%' class='tbl'>
                <tr>
                        <td class="caption">local redirect
                        <td class="caption">remote redirect
                <tr>
                        <td><input type="text" name="l_domain"><input type="button" value="add" onClick="AddOption(l_domains, l_domain.value)">
                        <td><input type="text" name="p_domain"><input type="button" value="add" onClick="AddOption(p_domains, p_domain.value)">
                <tr>
                        <td>
                                <select name="l_domains" size="5" multiple>
                                        <?php foreach($l_domains as $domain) { ?>
                                                <option value="<?php echo $domain; ?>"><?php echo $domain; ?></option>
                                        <?php } ?>
                                </select>
                        <td>
                                <select name="p_domains" size="5" multiple>
                                        <?php foreach($p_domains as $domain) { ?>
                                                <option value="<?php echo $domain; ?>"><?php echo $domain; ?></option>
                                        <?php } ?>
                                </select>
                <tr>
                        <td><input type="button" value="remove" onClick="DelOption(l_domains)">
                        <td><input type="button" value="remove" onClick="DelOption(p_domains)">
                <tr>
                        <td colspan="2" class="caption"><hr>custom mail processing
                <tr>
                        <td colspan="2">
                                <textarea name="advanced" rows="10" cols="80"><?php echo $advanced; ?></textarea><br><br>
                <tr>                  
                        <td colspan="2" align="right"><input type="submit" name="submit" value=" submit ">
        </table>
</form>
<hr>
<img src=gfx/ico_info.gif align=absmiddle title=info> use '&lt;tab&gt;' for real tabs
<hr>
<br><br>