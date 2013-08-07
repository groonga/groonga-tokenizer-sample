#include <groonga/tokenizer.h>

#include <string.h>
#include <stdio.h>

#ifdef __GNUC__
#  define GNUC_UNUSED __attribute__((__unused__))
#else
#  define GNUC_UNUSED
#endif

typedef struct {
  const char *next;
  int rest;
  grn_tokenizer_query *query;
  grn_tokenizer_token token;
} sample_tokenizer;


static grn_obj *
sample_init(grn_ctx *ctx, int nargs, grn_obj **args, grn_user_data *user_data)
{
  sample_tokenizer *tokenizer;
  unsigned int normalizer_flags = 0;
  grn_tokenizer_query *query;
  grn_obj *normalized_query;
  const char *normalized_string;
  unsigned int normalized_string_length;

  query = grn_tokenizer_query_open(ctx, nargs, args, normalizer_flags);
  if (!query) {
    return NULL;
  }

  tokenizer = GRN_PLUGIN_MALLOC(ctx, sizeof(sample_tokenizer));
  if (!tokenizer) {
    grn_tokenizer_query_close(ctx, query);
    GRN_PLUGIN_ERROR(ctx, GRN_NO_MEMORY_AVAILABLE,
                     "[tokenizer][sample] "
                     "memory allocation to sample_tokenizer failed");
    return NULL;
  }

  tokenizer->query = query;

  normalized_query = query->normalized_query;
  grn_string_get_normalized(ctx,
                            normalized_query,
                            &normalized_string,
                            &normalized_string_length,
                            NULL);
  tokenizer->next = normalized_string;
  tokenizer->rest = normalized_string_length;

  user_data->ptr = tokenizer;

  grn_tokenizer_token_init(ctx, &(tokenizer->token));

  return NULL;
}

static grn_obj *
sample_next(grn_ctx *ctx, GNUC_UNUSED int nargs, GNUC_UNUSED grn_obj **args,
            grn_user_data *user_data)
{
  sample_tokenizer *tokenizer = user_data->ptr;
  grn_encoding encoding = tokenizer->query->encoding;
  grn_tokenizer_status status;
  const char *token;
  int token_length;

  token = tokenizer->next;
  token_length = grn_plugin_charlen(ctx, token, tokenizer->rest, encoding);
  if (token_length == 0 || tokenizer->rest - token_length == 0) {
    status = GRN_TOKENIZER_LAST;
  } else {
    status = GRN_TOKENIZER_CONTINUE;
  }
  grn_tokenizer_token_push(ctx, &(tokenizer->token),
                           token, token_length, status);

  tokenizer->next += token_length;
  tokenizer->rest -= token_length;

  return NULL;
}

static grn_obj *
sample_fin(grn_ctx *ctx, GNUC_UNUSED int nargs, GNUC_UNUSED grn_obj **args,
           grn_user_data *user_data)
{
  sample_tokenizer *tokenizer = user_data->ptr;

  if (!tokenizer) {
    return NULL;
  }

  grn_tokenizer_token_fin(ctx, &(tokenizer->token));
  grn_tokenizer_query_close(ctx, tokenizer->query);
  GRN_PLUGIN_FREE(ctx, tokenizer);

  return NULL;
}


grn_rc
GRN_PLUGIN_INIT(grn_ctx *ctx)
{
  return ctx->rc;
}

grn_rc
GRN_PLUGIN_REGISTER(grn_ctx *ctx)
{
  grn_rc rc;

  rc = grn_tokenizer_register(ctx, "TokenSample", -1,
                              sample_init, sample_next, sample_fin);

  return rc;
}

grn_rc
GRN_PLUGIN_FIN(GNUC_UNUSED grn_ctx *ctx)
{
  return GRN_SUCCESS;
}
