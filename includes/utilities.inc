<?php

/**
 * @file
 * This file contains helper functions used throughout this module.
 */

/**
 * Checks if it is possible to create files with tesseract.
 *
 * @return bool
 *   TRUE if it is possible, FALSE otherwise.
 */
function islandora_ocr_can_derive_ocr() {
  $version = islandora_ocr_get_tesseract_version();
  return version_compare($version, TESSERACT_REQUIRED_VERSION) >= 0;
}

/**
 * Gets the versions of the given tesseract executable.
 *
 * @param string $tesseract
 *   The absolute path to $tesseract executable, if not provided the variable
 *   "islandora_ocr_tesseract" will be used instead.
 *
 * @return string
 *   The version of the tesseract executable if successful, FALSE otherwise.
 */
function islandora_ocr_get_tesseract_version($tesseract = NULL) {
  $tesseract = $tesseract ? $tesseract : variable_get('islandora_ocr_tesseract', '/usr/bin/tesseract');
  if (is_executable($tesseract)) {
    $ret = -1;
    $output = array();
    $command = "$tesseract -v 2>&1";
    exec($command, $output, $ret);
    if (!$ret && isset($output[0])) {
      return str_replace(array('tesseract', ' '), '', $output[0]);
    }
  }
  return FALSE;
}

/**
 * Gets tesseracts installed languages.
 *
 * @param string $tesseract
 *   The absolute path to $tesseract executable, if not provided the variable
 *   "islandora_ocr_tesseract" will be used instead.
 *
 * @return array
 *   The installed languages where the value is in a form tesseract recognizes.
 */
function islandora_ocr_get_tesseract_installed_languages($tesseract = NULL) {
  $tesseract = $tesseract ? $tesseract : variable_get('islandora_ocr_tesseract', '/usr/bin/tesseract');
  if (is_executable($tesseract)) {
    $ret = -1;
    $output = array();
    $command = "$tesseract --list-langs 2>&1";
    exec($command, $output, $ret);
    if (!$ret && count($output) > 1) {
      // Remove the descriptive first line, "List of available languages (n):"
      array_shift($output);
      return $output;
    }
  }
  return array();
}

/**
 * Gets the human readable name for the given tesseract language abbreviation.
 *
 * @param string $language
 *   A tesseract recognized language abbreviation.
 *
 * @return string
 *   A translated human readable name for the given abbreviation if found,
 *   otherwise the abbreviation is returned unaltered.
 */
function islandora_ocr_tesseract_language_name($language) {
  $language_names = array(
    'eng' => t('English'),
    'fra' => t('French'),
    'deu-frak' => t('German'),
    'por' => t('Portugese'),
    'spa' => t('Spanish'),
    'hin' => t('Hindi'),
    'jpn' => t('Japanese'),
    'ita' => t('Italian'),
  );
  return isset($language_names[$language]) ? $language_names[$language] : $language;
}

/**
 * Gets only the enabled languages as defined by the user in the admin form.
 *
 * @return array
 *   The enabled languages where the key is the value tesseract recognizes and
 *   the value is a human readable version of the language.
 */
function islandora_ocr_get_enabled_tesseract_languages() {
  $enabled_languages = array_filter(variable_get('islandora_ocr_tesseract_enabled_languages'));
  return drupal_map_assoc($enabled_languages, 'islandora_ocr_tesseract_language_name');
}
