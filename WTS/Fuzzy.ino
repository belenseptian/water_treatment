void initFuzzy()
{
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

float fuzzyOut(float ppm, float ph)
{
  fuzzy->setInput(1, ph);
  fuzzy->setInput(2, ppm);
  float output1 = fuzzy->defuzzify(1);

  return output1;
}
