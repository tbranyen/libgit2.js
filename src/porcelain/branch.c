#include <git2.h>
#include <stdio.h>
#include <string.h>

int branch(char* branch_name) {
  git_reference* out;
  git_repository* repo;
  git_oid head_oid;
  git_commit* head_commit;

  git_repository_open(&repo, ".");

  git_reference_name_to_id(&head_oid, repo, "HEAD");
  git_commit_lookup(&head_commit, repo, &head_oid);

  return git_branch_create(&out, repo, branch_name, head_commit, 1);
}

int checkout_ref(char* ref_name) {
  git_repository* repo;
  git_repository_open(&repo, ".");

  git_checkout_opts opts = GIT_CHECKOUT_OPTS_INIT;
  opts.checkout_strategy = GIT_CHECKOUT_FORCE;

  if (git_repository_set_head(repo, ref_name) != 0) {
    return -1;
  }
  return git_checkout_head(repo, &opts);
}

int checkout_sha_prefix(char* sha) {
  git_repository* repo;
  git_repository_open(&repo, ".");

  git_object* commit;
  git_oid commit_oid;

  git_checkout_opts opts = GIT_CHECKOUT_OPTS_INIT;
  opts.checkout_strategy = GIT_CHECKOUT_FORCE;

  git_oid_fromstr(&commit_oid, sha);
  git_object_lookup_prefix(&commit, repo, &commit_oid, strlen(sha), GIT_OBJ_COMMIT);
  return git_checkout_tree(repo, commit, &opts);
}

char const * get_head_name() {
  git_repository* repo;
  git_repository_open(&repo, ".");
  char const * ret;

  git_reference* HEAD;
  git_repository_head(&HEAD, repo);

  ret = git_reference_name(HEAD);
  git_reference_free(HEAD);
  return ret;
}