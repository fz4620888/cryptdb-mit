#include <stdio.h>
#include <bsd/string.h>

#include "sql_priv.h"
#include "unireg.h"
#include "strfunc.h"
#include "sql_class.h"
#include "set_var.h"
#include "sql_base.h"
#include "rpl_handler.h"
#include "sql_parse.h"
#include "sql_plugin.h"

extern int MYSQLparse(void *thd); // from sql_yacc.cc

static void
parse(const char *q)
{
  my_thread_init();
  THD *t = new THD;
  if (t->store_globals())
    printf("store_globals error\n");

  char buf[1024];
  size_t len = strlcpy(buf, q, sizeof(buf));

  Parser_state ps;
  if (!ps.init(t, buf, len)) {
    LEX lex;
    t->lex = &lex;

    lex_start(t);
    bool x = parse_sql(t, &ps, 0);
    t->end_statement();

    printf("q=%s\n", q);
    printf("x=%s\n", x?"true":"false");
  } else {
    printf("parser init error\n");
  }

  t->cleanup_after_query();
  delete t;
}

int
main(int ac, char **av)
{
  system_charset_info = &my_charset_utf8_general_ci;
  global_system_variables.character_set_client = system_charset_info;

  pthread_key_t dummy;
  if (pthread_key_create(&dummy, 0) ||
      pthread_key_create(&THR_THD, 0) ||
      pthread_key_create(&THR_MALLOC, 0))
    printf("pthread_key_create error\n");

  sys_var_init();
  lex_init();
  item_create_init();
  item_init();

  my_init();
  mdl_init();
  table_def_init();
  randominit(&sql_rand, 0, 0);
  delegates_init();
  init_tmpdir(&mysql_tmpdir_list, 0);

  default_charset_info = get_charset_by_csname("utf8", MY_CS_PRIMARY, MYF(MY_WME));
  global_system_variables.collation_server         = default_charset_info;
  global_system_variables.collation_database       = default_charset_info;
  global_system_variables.collation_connection     = default_charset_info;
  global_system_variables.character_set_results    = default_charset_info;
  global_system_variables.character_set_client     = default_charset_info;
  global_system_variables.character_set_filesystem = default_charset_info;

  opt_ignore_builtin_innodb = true;
  int plugin_ac = 1;
  char *plugin_av = (char *) "x";
  plugin_init(&plugin_ac, &plugin_av, 0);

  //const char *engine = "MEMORY";
  //LEX_STRING name = { (char *) engine, strlen(engine) };
  //plugin_ref plugin = ha_resolve_by_name(0, &name);
  //global_system_variables.table_plugin = plugin;

  const char *q = "SELECT * FROM t";
  parse(q);
}