#include "../Headers/programm.hpp"

Programm::Programm()
{
	this -> source_path = 0;
	this -> output_path = 0;
	this -> source_text = 0;
	this -> text_length = 0;
	this -> source_tokens = 0;
	this -> tokens_length = 0;
	this -> programm_tree = 0;
	this -> compiled_text = 0;
}

Programm::Programm
