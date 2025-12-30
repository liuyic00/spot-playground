#include <fstream>
#include <iostream>
#include <spot/tl/print.hh>
#include <spot/twa/twagraph.hh>
#include <spot/twaalgos/neverclaim.hh>
#include <spot/twaalgos/sbacc.hh>
#include <spot/twaalgos/split.hh>

#include <spot/tl/formula.hh>
#include <spot/tl/parse.hh>
#include <spot/twaalgos/dot.hh>
#include <spot/twaalgos/hoa.hh>
#include <spot/twaalgos/translate.hh>

void print_ba_format(std::ostream &out, spot::twa_graph_ptr aut) {
  // The input should have Büchi acceptance.  Alternatively,
  // allow "t" acceptance since we can interpret this as a Büchi automaton
  // where all states are accepting.
  const spot::acc_cond &acc = aut->acc();
  if (!(acc.is_buchi() || acc.is_t()))
    throw std::runtime_error("unsupported acceptance condition");

  // The BA format only support state-based acceptance, so get rid
  // of transition-based acceptance if we have some.
  aut = spot::sbacc(aut);

  // We want one minterm per edge, as those will become letters
  aut = spot::split_edges(aut);

  out << aut->get_init_state_number() << '\n';
  for (auto &e : aut->edges())
    out << e.cond.id() << ',' << e.src << "->" << e.dst << '\n';

  unsigned ns = aut->num_states();
  for (unsigned s = 0; s < ns; ++s)
    if (acc.accepting(aut->state_acc_sets(s)))
      out << s << '\n';
}

int main() {

  // construct the formula
  spot::formula fga =
      spot::formula::F(spot::formula::G(spot::formula::ap("b")));

  // translate it to an automaton
  spot::translator trans;
  trans.set_type(spot::postprocessor::Buchi);
  trans.set_pref(spot::postprocessor::SBAcc |
                 spot::postprocessor::Deterministic);
  trans.set_level(spot::postprocessor::High);
  spot::twa_graph_ptr aut = trans.run(fga);

  // print the automaton
  // DOT format
  system("mkdir -p outs/transform");
  std::ofstream fout("outs/transform/transform.dot");
  spot::print_dot(fout, aut);
  fout.close();
  // HOA format
  spot::print_hoa(std::cout, aut) << '\n';
  // custom BA format
  print_ba_format(std::cout, aut);

  return 0;
}
