<?php

function boucado_theme() {
  $items = array();
/*
  $items['user_register_form'] = array(
    'render element' => 'form',
    'template' => 'user-register-form',
    'path' => drupal_get_path('module', 'boucado'),    
    'preprocess functions' => array('boucado_preprocess_user_register_form'),
  );
 /*    
  $items['user_login'] = array(
    'render element' => 'form',
    'path' => drupal_get_path('theme', 'yourtheme') . '/templates',
    'template' => 'user-login',
    'preprocess functions' => array(
       'yourtheme_preprocess_user_login'
    ),
  );
  $items['user_pass'] = array(
    'render element' => 'form',
    'path' => drupal_get_path('theme', 'yourtheme') . '/templates',
    'template' => 'user-pass',
    'preprocess functions' => array(
      'yourtheme_preprocess_user_pass'
    ),
  );
  * */
  return $items;
 }

function boucado_preprocess_user_register_form(&$vars) {
  $vars['intro_text'] = t('Welcome at BOUCADO!');
}


function boucado_preprocess_user_login(&$vars) {
  $vars['intro_text'] = t('This is my awesome login form');
}

function boucado_preprocess_user_pass(&$vars) {
  $vars['intro_text'] = t('This is my super awesome request new password form');
}


function boucado_form_alter(&$form, &$form_state, $form_id) {
global $user;	

// drupal_set_message($form_id);

  if ($form_id == 'search_block_form' || $form_id == 'search_form') {
    $form['#attributes'] = array('class' => array('art-search')); // Add Artisteer class
    $form['actions']['submit']['#attributes'] = array('class' => array('art-search-button')); // Add Artisteer class
  }

// ******** user_register

    if ($form_id == 'user_register_form') {
	
	$sponsor_name = '';
	if (isset($_COOKIE['sponsor_name'])) {
		$sponsor_name = $_COOKIE['sponsor_name']; 
	} else if (isset($_SESSION['sponsor_name'])){
		$sponsor_name = $_SESSION['sponsor_name'];
	} 


	$form['account']['name'] = array(
	    '#title' => t('username'),
	    '#description' => 'Wählen Sie hier einen Benutzernamen nach Ihren Wünschen, aber beachten Sie unten angeführte Regeln. Der Benutzername ist sehr wichtig für Sie und kann nicht mehr geändert werden! Einerseits benötigen Sie ihn, um sich bei BOUCADO einzuloggen. 
Andererseits wird dieser Name verwendet, wenn Sie weitere Personen als Mitglieder anwerben, damit das System weiß, wer wen geworben hat und wir dadurch die Werbeprämie an Sie ausbezahlen können. <br><b>Regeln:</b> Der Benutzername muss zwischen 3 und 15 Zeichen lang sein und darf keine Leerzeichen, Satzzeichen und Sonderzeichen enthalten. Beispiele: RUDI1963, RudiPur, Winner1.<br>Sollte ein Benutzername bereits im System vorhanden sein, dann müssen Sie einen anderen wählen.<br>WÄHLEN SIE DEN BENUTZERNAMEN MIT BEDACHT! ER KANN SPÄTER NICHT MEHR GEÄNDERT WERDEN!',
	    '#type' => 'textfield',
	    '#required' => TRUE,
	    '#weight' => -1,
	);
	
	  
	$form['benutzer'] = array(
	  '#type' => 'fieldset',
	  '#title' => t('BOUCADO Informationen'),
	  '#weight' => 1,
	);
	
   
	$form['benutzer']['sponsor'] = array(
	  '#title' => t('Sponsor'),
	  '#description' => 'Der Sponsor ist jene Person, welche Sie als Mitglied bei BOUCADO anwerben möchte. Wenn das Feld leer ist, dann müssen Sie hier den Benutzernamen der Person einfügen, die Ihnen BOUCADO empfohlen hat.',
	  '#type' => 'textfield',
	  '#element_validate' => array('boucado_sponsor_validate'),
	  '#required' => TRUE,
	  '#default_value' => $sponsor_name,
	  '#weight' => 1,
	);

	$form['bedingungen'] = array(
	  '#type' => 'fieldset',
	  '#title' => t('Bedingungen'),
	  '#weight' => 5,
	);
	
	$form['bedingungen']['AGB'] = array(
	  '#type' => 'checkbox',
	  '#title' => 'Ich habe die <a href="/agb">Allgemeinen Geschäftsbedingungen</a> gelesen und akzeptiere sie.',
	  '#element_validate' => array('boucado_agb_validate'),
	  '#weight' => 2,
	);

    //$form['#validate'][] = 'boucado_user_register_form_validate';
	$form['#submit'][] = 'boucado_user_register_form_submit';
	//$form['#submit']['#weight'] = 10;
    }

// ******** user_profile_form

	if ($form_id == 'user_profile_form') {
	$account_id = arg(1);
	$account = user_load($account_id);
	$user_id = $account->uid;
	$user_name = $account->name;
	$userdata = boucado_get_userdata($user_name);
	
	$form['KundenID_online'] = array(
		'#type' => 'hidden', 
		'#value' => $userdata->member_id,
	);
	
	$form['personal'] = array(
	  '#type' => 'fieldset',
	  '#title' => 'Kontakt Informationen',
	  '#weight' => 1,
	);
	$form['personal']['strTitel_O'] = array(
	  '#type' => 'textfield',
	  '#title' => t('Title'),
	  '#required' => TRUE,
	  '#default_value' => $userdata->titel,	  
	  '#weight' => 2,	 
	);		
	$form['personal']['strVName_O'] = array(
	  '#type' => 'textfield',
	  '#title' => t('Firstname'),
	  '#required' => TRUE,
	  '#default_value' => $userdata->first_name,	  
	  '#weight' => 2,	 
	);	
	$form['personal']['strNName_O'] = array(
	  '#type' => 'textfield',
	  '#title' => t('Lastname'),
	  '#required' => TRUE,
	  '#default_value' => $userdata->last_name,	  
	  '#weight' => 3,	  
	);
	$form['personal']['strAnschrift_O'] = array(
	  '#type' => 'textfield',
	  '#title' => t('Street'),
	  '#required' => TRUE,	 
	  '#default_value' => $userdata->street,	   
	  '#weight' => 4,	 
	);
	$form['personal']['strPLZ_O'] = array(
	  '#type' => 'textfield',
	  '#title' => t('Postcode'),
	  '#required' => TRUE,
	  '#default_value' => $userdata->postal,	  
	  '#weight' => 5,	  
	);
	$form['personal']['strOrt_O'] = array(
	  '#type' => 'textfield',
	  '#title' => t('City'),
	  '#required' => TRUE,
	  '#default_value' => $userdata->city,	  
	  '#weight' => 6,	  
	);
	$form['personal']['strLand_O'] = array(
	  '#type' => 'select',
	  '#title' => t('Country'),
	  '#options' => array('' => '', 'AT' => 'Österreich','DE' => 'Deutschland'),
	  '#required' => TRUE,
	  '#default_value' => $userdata->country,	  
	  '#weight' => 7,	  
	);
	$form['personal']['strTelefon_O'] = array(
	  '#type' => 'textfield',
	  '#title' => t('Phone'),
	  '#required' => TRUE,
	  '#default_value' => $userdata->phone,	  
	  '#weight' => 9,	
	);
	$form['personal']['account_id'] = array(
	  '#type' => 'textfield',
	  '#title' => t('PayPal ID'),
	  '#required' => FALSE,
	  '#default_value' => $userdata->account_id,	  
	  '#weight' => 10,	
	);

	$form['#submit'][] = 'boucado_user_edit_form_submit';
	unset($form['contact']);

	}
}

/**
* Preprocess user profile form.
*/
function boucado_preprocess_user_profile_form(&$vars) {

  // Uncomment the following line if Devel module is enabled, to view the contents of the form.
  dsm($vars['form']);

  // Change the help text for specific form elements.
  $vars['form']['account']['name']['#description'] = t('Custom description regarding the Username.');

  // Adjust the titles of several fieldsets.
  $vars['form']['picture']['#title'] = t('Your user picture / avatar');
  $vars['form']['timezone']['#title'] = t('Time zone');
  unset($vars['form']['timezone']['timezone']['#title']);

  // Set several elements that by default have collapsed fieldsets to expanded and non-collapsible.
  $vars['form']['theme_select']['themes']['#collapsible'] = FALSE;
  $vars['form']['picture']['#collapsible'] = FALSE;
  $vars['form']['contact']['#collapsible'] = FALSE;
  $vars['form']['timezone']['#collapsible'] = FALSE;

  // Adjust the size of several fields to fit better in 2 columns.
  $vars['form']['account']['name']['#size'] = 25;
  $vars['form']['account']['mail']['#size'] = 25;
  $vars['form']['picture']['picture_upload']['#size'] = 40;
  $vars['form']['signature_settings']['signature']['#cols'] = 50;

  // Rename the Save and Delete buttons to be more clear.
  $vars['form']['submit']['#value'] = t('Profil speichern');
  $vars['form']['delete']['#value'] = t('Konto kündigen');


  // Prepare all of the desired form elements as variables, to be used in user-profile-form.tpl.php.
  // Everything before this part is optional.
  $vars['account'] = drupal_render($vars['form']['account']);
  $vars['theme_select'] = drupal_render($vars['form']['theme_select']);
  $vars['picture'] = drupal_render($vars['form']['picture']);
  $vars['signature_settings'] = drupal_render($vars['form']['signature_settings']);
  $vars['contact'] = drupal_render($vars['form']['contact']);
  $vars['timezone'] = drupal_render($vars['form']['timezone']);
  $vars['submit'] = drupal_render($vars['form']['submit']);
  $vars['delete'] = drupal_render($vars['form']['delete']);

}

function boucado_sponsor_validate($element, &$form_state) {
  $sponsor = $element['#value']; 
  if(!boucado_check_sponsor($sponsor)) {
    form_error($element, t('Sponsor "%s" ist unbekannt.', array('%s'=>$sponsor)));
  }
}

function boucado_password_validate($element, &$form_state) {
  $password = $element['#value'];
  $password1 = $form_state['values']['password1'];
  if($password != $password1) {
    form_error($element, 'Passwörter sind nicht gleich!');
  }
}

function boucado_geburtstag_validate($element, &$form_state) {
  $geburtstag = $element['#value']; 
  if(date('Y',time()) - $geburtstag['year'] < 1) {
    form_error($element, t('Das Geburtsjahr "%d" ist ungültig. Das Mindestalter ist 1 Jahr!',array('%d' => $geburtstag['year'])));
  }
  if(date('Y',time()) - $geburtstag['year'] > 74) {
    form_error($element, t('Das Geburtsjahr "%d" ist ungültig. Das Höchstalter ist 75 Jahre!',array('%d' => $geburtstag['year'])));
  }  
}

function boucado_iban_validate($element, &$form_state) {
  $iban = $element['#value'];
  if (!verify_iban($iban)) {
    form_error($element, t('IBAN ist ungültig.'));
  }
}

function boucado_bic_validate($element, &$form_state) {
	
	if(!eregi("^([a-zA-Z]){4}([a-zA-Z]){2}([0-9a-zA-Z]){2}([0-9a-zA-Z]{3})?$", $element['#value'])) {
		form_error($element, t('BIC ist ungültig.'));
	} 
}

function boucado_agb_validate($element, &$form_state) {
  if(empty($element['#value'])) {
    form_error($element, t('Sie müssen den Allgemeinen Geschäftsbedingungen zustimmen!'));
  }
}

function boucado_beitrag_validate($element, &$form_state) {
  if(empty($element['#value'])) {
    form_error($element, t('Sie müssen dem Einzugsverfahren zustimmen!'));
  }
}

function boucado_user_register_form_submit($form, &$form_state) {
	// $form_state['redirect']= 'user';
	$username = $form_state['values']['name'];
	$sponsor = boucado_get_userdata($form_state['values']['sponsor']);
	$email = $form_state['values']['mail'];

	db_set_active('boucado_matrix');
	$newuser = db_query("SELECT * FROM {members} WHERE username='" . $username . "'");
	if (!$newuser->rowCount()) {

    	$success = db_query("INSERT INTO {members} (username,enroller_id,email,reg_date,m_level,is_active,is_dead) 
		VALUES ('" . $username . "','" . $sponsor->member_id . "','" .	$email . "'," . time() . ",0,0,0)");
		if (!$success) {
			drupal_set_message("Der Benutzer wurde NICHT gespeichert.", 'error');
		}
		else {
			drupal_set_message("Der Benutzer wurde gespeichert.");
		}
	}
	
	db_set_active();
	

/*
	// E-Mail an Upline senden
	$level = 1;

	$newmember = boucado_get_userdata($username);
	$upline_name = $sponsor;
	$upline_id = $newmember->enroller_id;
	// solange nicht oben angekommen und Level < 10: 
	while ($upline_id > 0 && $level < 10) {

		$upline = boucado_get_userdata($upline_name);
		$upline_id = $upline->enroller_id;
		$upline_name = boucado_get_username($upline->enroller_id);  
		
		$params = array(
			'Uplinename' => $upline->first_name,
	  		'Sponsorname' => $sponsor->username,
	  		'Neueruser' => $username
		);
		
		$to = $upline->email;
		$sent = drupal_mail('boucado', 'upline', $to, language_default(), $params, 'office@boucado.space');
		
		$level++;
	}
		
	$to = "office@boucado.space";
	$sent = drupal_mail('boucado', 'neuer', $to, language_default(), $params, 'office@boucado.space');

 * 
 */
 }

function boucado_user_edit_form_submit($form, &$form_state) {

	$KundenID_online = $form_state['values']['KundenID_online'];
	$strEmail_O = $form_state['values']['mail'];
	$strNName_O = $form_state['values']['strNName_O'];
	$strTitel_O = $form_state['values']['strTitel_O'];	
	$strVName_O = $form_state['values']['strVName_O'];
	$strAnschrift_O = $form_state['values']['strAnschrift_O'];
	$strPLZ_O = $form_state['values']['strPLZ_O'];
	$strOrt_O = $form_state['values']['strOrt_O'];
	$strLand_O = $form_state['values']['strLand_O'];
	$strTelefon_O = $form_state['values']['strTelefon_O'];
	$account_id = $form_state['values']['account_id'];

	db_set_active('boucado_matrix');
	$sql = "UPDATE {members} SET 
	email='" . $strEmail_O . 
	"',titel='" . $strTitel_O .	
	"',last_name='" . $strNName_O . 
	"',first_name='" . $strVName_O . 
	"',street='" . $strAnschrift_O . 
	"',postal='" . $strPLZ_O . 
	"',city='" . $strOrt_O . 
	"',country='" . $strLand_O . 
	"',phone='" . $strTelefon_O . 
	"',account_id='" . $account_id . 
	"' WHERE member_id=" . $KundenID_online;
	
	$success = db_query($sql);

	if (!$success) {
		drupal_set_message("Der Benutzer wurde NICHT gespeichert.", 'error');
		db_set_active(); 
		return false;
	}
	else {
		drupal_set_message("Der Benutzer wurde gespeichert.");
		db_set_active();
		drupal_goto("user"); 
	}
	
}

?>