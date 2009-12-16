<?php
// $Id$

/**
 * @file Invokes events on behalf core modules. Usually this should be
 *   directly in the module providing rules integration instead.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Node events
 * For getting the unchanged node we currently have to implement a workaround.
 * If http://drupal.org/node/651240 gets in, we can simplify that.
 */

function rules_node_view($node, $build_mode) {
  rules_invoke_event('node_view', $node, $build_mode);
}

function rules_node_presave($node) {
  $nodes = &drupal_static('rules_node_unchanged', array());
  if (!empty($node->nid)) {
    // Make sure node_load() has statically cached the unchanged object, so we
    // can still retrieve after update. See rules_events_node_var_unchanged().
    array_push($nodes, clone node_load($node->nid));
  }
  rules_invoke_event('node_presave', $node);
}

function rules_node_update($node) {
  rules_invoke_event('node_update', $node);
  // Pop the unchanged node so recursive invocations work right.
  $nodes = &drupal_static('rules_node_unchanged', array());
  array_pop($nodes);
}

function rules_node_insert($node) {
  rules_invoke_event('node_insert', $node);
}

function rules_node_delete($node) {
  rules_invoke_event('node_delete', $node);
}

/**
 * Gets the node object, that doesn't contain the changes.
 *
 * @see rules_events_node_variables()
 */
function rules_events_node_var_unchanged($node) {
  if (empty($node->nid)) {
    throw new RulesException("Can't load the unchanged node variable as there is none yet.");
  }
  // Use read-only so actually don't change the array.
  $nodes = drupal_static('rules_node_unchanged', array());
  return array_pop($nodes);
}


/**
 * Handler for loading the acting user variable, which is always the global
 * user.
 *
 * @see rules_events_global_user_parameter()
 */
function rules_events_parameter_global_user() {
  global $user;
  return user_load(array('uid' => $user->uid));
}

/**
 * Invoke system events.
 */
function rules_init() {
  rules_invoke_event('init');
}

function rules_cron() {
  rules_invoke_event('cron');
}


/**
 * @}
 */