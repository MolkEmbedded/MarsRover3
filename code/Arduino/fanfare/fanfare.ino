

#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047

#define SONG_LENGTH 14
#define BUZZERPORT 6

// notes in the melody:
int melody[] = {
  NOTE_C5, NOTE_C5, NOTE_C5, NOTE_G5, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_G5,NOTE_G5, NOTE_G5, NOTE_G5, NOTE_C5,NOTE_C5, NOTE_G5
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  8, 16, 16, 4, 8, 16, 16, 4, 8, 16, 16, 8, 8, 4
};

void playFanfare(){
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < SONG_LENGTH; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(SONG_LENGTH, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(SONG_LENGTH);
  }
}

// obstacle
// set notes
int obstacleNoteDuration[] = {8,8};
int obstacleMelody[] = {NOTE_G5, NOTE_C5};

void playObstacleMelody() {
  for( uint8_t thisNote = 0; thisNote < 2; thisNote++){
    int noteDuration = 1000 / obstacleNoteDuration[thisNote];
    tone(2, obstacleMelody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(2);
  }
}

int startMelody[] = {NOTE_C5,0, NOTE_C5, 0, NOTE_C5, 0, NOTE_C5};
int startDuration[] = {8,8,8,8,8,8,2};
void playStart(){
  for( uint8_t thisNote = 0; thisNote < 2; thisNote++){
    int noteDuration = 1000 / startDuration[thisNote];
    tone(BUZZERPORT, startMelody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(BUZZERPORT);
  }
}
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
