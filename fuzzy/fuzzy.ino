#include <Fuzzy.h>

// Fuzzy
Fuzzy *fuzzy = new Fuzzy();

// FuzzyInput PH Sensor
FuzzySet *phfair = new FuzzySet(5.5, 6, 6, 7);
FuzzySet *phgood = new FuzzySet(6.5, 7, 8, 8.5);
FuzzySet *phbad = new FuzzySet(8, 8.5, 8.5, 10.5);

// FuzzyInput TDS
FuzzySet *excellent = new FuzzySet(150, 250, 250, 300);
FuzzySet *good = new FuzzySet(250, 300, 500, 600);
FuzzySet *fair = new FuzzySet(500, 600, 600, 900);
 
// FuzzyOutput water quality
FuzzySet *bad = new FuzzySet(0, 40, 40, 45);
FuzzySet *medium = new FuzzySet(40, 45, 70, 75);
FuzzySet *wqgood = new FuzzySet(70, 75, 75, 100);

void setup()
{
  // Set the Serial output
  Serial.begin(9600);
  // Set a random seed
  randomSeed(analogRead(0));

  // Every setup must occur in the function setup()

  // FuzzyInput
  FuzzyInput *ph = new FuzzyInput(1);

  ph->addFuzzySet(phfair);
  ph->addFuzzySet(phgood);
  ph->addFuzzySet(phbad);
  fuzzy->addFuzzyInput(ph);

  // FuzzyInput
  FuzzyInput *tds = new FuzzyInput(2);

  tds->addFuzzySet(excellent);
  tds->addFuzzySet(good);
  tds->addFuzzySet(fair);
  fuzzy->addFuzzyInput(tds);

  // FuzzyOutput
  FuzzyOutput *quality = new FuzzyOutput(1);

  quality->addFuzzySet(bad);
  quality->addFuzzySet(medium);
  quality->addFuzzySet(wqgood);
  fuzzy->addFuzzyOutput(quality);

  // Building FuzzyRule #1
  FuzzyRuleAntecedent *iffairPHAndExcellentTDS = new FuzzyRuleAntecedent();
  iffairPHAndExcellentTDS->joinWithAND(phfair, excellent);

  FuzzyRuleConsequent *thenWQMedium = new FuzzyRuleConsequent();
  thenWQMedium->addOutput(medium);

  FuzzyRule *fuzzyRule1 = new FuzzyRule(1, iffairPHAndExcellentTDS, thenWQMedium);
  fuzzy->addFuzzyRule(fuzzyRule1);

  // Building FuzzyRule #2
  FuzzyRuleAntecedent *iffairPHAndGoodTDS = new FuzzyRuleAntecedent();
  iffairPHAndGoodTDS->joinWithAND(phfair, good);

  FuzzyRuleConsequent *thenWQBad2 = new FuzzyRuleConsequent();
  thenWQBad2->addOutput(bad);

  FuzzyRule *fuzzyRule2 = new FuzzyRule(2, iffairPHAndGoodTDS, thenWQBad2);
  fuzzy->addFuzzyRule(fuzzyRule2);

  // Building FuzzyRule #3
  FuzzyRuleAntecedent *iffairPHAndFairTDS = new FuzzyRuleAntecedent();
  iffairPHAndFairTDS->joinWithAND(phfair, fair);

  FuzzyRuleConsequent *thenWQBad3 = new FuzzyRuleConsequent();
  thenWQBad3->addOutput(bad);

  FuzzyRule *fuzzyRule3 = new FuzzyRule(3, iffairPHAndFairTDS, thenWQBad3);
  fuzzy->addFuzzyRule(fuzzyRule3);

  // Building FuzzyRule #4
  FuzzyRuleAntecedent *ifgoodPHAndExcellentsTDS = new FuzzyRuleAntecedent();
  ifgoodPHAndExcellentsTDS->joinWithAND(phgood, excellent);

  FuzzyRuleConsequent *thenWQGood = new FuzzyRuleConsequent();
  thenWQGood->addOutput(wqgood);

  FuzzyRule *fuzzyRule4 = new FuzzyRule(4, ifgoodPHAndExcellentsTDS, thenWQGood);
  fuzzy->addFuzzyRule(fuzzyRule4);

  // Building FuzzyRule #5
  FuzzyRuleAntecedent *ifgoodPHAndGoodTDS = new FuzzyRuleAntecedent();
  ifgoodPHAndGoodTDS->joinWithAND(phgood, good);

  FuzzyRuleConsequent *thenWQGood5 = new FuzzyRuleConsequent();
  thenWQGood5->addOutput(wqgood);

  FuzzyRule *fuzzyRule5 = new FuzzyRule(5, ifgoodPHAndGoodTDS, thenWQGood5);
  fuzzy->addFuzzyRule(fuzzyRule5);

  // Building FuzzyRule #6
  FuzzyRuleAntecedent *ifgoodPHAndFairTDS = new FuzzyRuleAntecedent();
  ifgoodPHAndFairTDS->joinWithAND(phgood, fair);

  FuzzyRuleConsequent *thenWQMedium2 = new FuzzyRuleConsequent();
  thenWQMedium2->addOutput(medium);

  FuzzyRule *fuzzyRule6 = new FuzzyRule(6, ifgoodPHAndFairTDS, thenWQMedium2);
  fuzzy->addFuzzyRule(fuzzyRule6);

  // Building FuzzyRule #7
  FuzzyRuleAntecedent *ifbadPHAndExcellentTDS = new FuzzyRuleAntecedent();
  ifbadPHAndExcellentTDS->joinWithAND(phbad, excellent);

  FuzzyRuleConsequent *thenWQBad7 = new FuzzyRuleConsequent();
  thenWQBad7->addOutput(bad);

  FuzzyRule *fuzzyRule7 = new FuzzyRule(7, ifbadPHAndExcellentTDS, thenWQBad7);
  fuzzy->addFuzzyRule(fuzzyRule7);
  
  // Building FuzzyRule #8
  FuzzyRuleAntecedent *ifbadPHAndGoodTDS = new FuzzyRuleAntecedent();
  ifbadPHAndGoodTDS->joinWithAND(phbad, good);

  FuzzyRuleConsequent *thenWQBad8 = new FuzzyRuleConsequent();
  thenWQBad8->addOutput(bad);

  FuzzyRule *fuzzyRule8 = new FuzzyRule(8, ifbadPHAndGoodTDS, thenWQBad8);
  fuzzy->addFuzzyRule(fuzzyRule8);

  // Building FuzzyRule #9
  FuzzyRuleAntecedent *ifbadPHAndFairTDS = new FuzzyRuleAntecedent();
  ifbadPHAndFairTDS->joinWithAND(phbad, fair);

  FuzzyRuleConsequent *thenWQBad9 = new FuzzyRuleConsequent();
  thenWQBad9->addOutput(bad);

  FuzzyRule *fuzzyRule9 = new FuzzyRule(9, ifbadPHAndFairTDS, thenWQBad9);
  fuzzy->addFuzzyRule(fuzzyRule9);
  
}

void loop()
{
  // get random entrances
  int input1 = random(6, 10.5);
  int input2 = random(150, 900);

  Serial.println("\n\n\nEntrance: ");
  Serial.print("\t\t\tPH: ");
  Serial.print(input1);
  Serial.print(", and TDS: ");
  Serial.println(input2);

  fuzzy->setInput(1, input1);
  fuzzy->setInput(2, input2);

  fuzzy->fuzzify();

  Serial.println("Input: ");
  Serial.print("\tPH: Fair-> ");
  Serial.print(phfair->getPertinence());
  Serial.print(", Good-> ");
  Serial.print(phgood->getPertinence());
  Serial.print(", Bad-> ");
  Serial.println(phbad->getPertinence());

  Serial.print("\tTDS: Excellent-> ");
  Serial.print(excellent->getPertinence());
  Serial.print(", Good-> ");
  Serial.print(good->getPertinence());
  Serial.print(", Fair-> ");
  Serial.println(fair->getPertinence());

  float output1 = fuzzy->defuzzify(1);

  Serial.println("Output: ");
  Serial.print("\tQuality: Bad-> ");
  Serial.print(bad->getPertinence());
  Serial.print(", Medium-> ");
  Serial.print(medium->getPertinence());
  Serial.print(", Good-> ");
  Serial.println(wqgood->getPertinence());

  Serial.println("Result: ");
  Serial.print("\t\t\tWater_quality: ");
  Serial.println(output1);

  delay(10000);
}
