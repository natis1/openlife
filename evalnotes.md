# Post eval notes

    Don't assume judges know Conway's game of life or boids.

    Know Conway's rules and Boids rules precisely.

    What is significance of Life or what you are doing or Boids?

    What application? How does this relate to real life?

    Could use K factor complexity to measure spacial complexity.

    Demonstrate variety based only on changing parameters. Have screen shots.

    What questions? hypotheses? what can you test with this model?

    Swarm robot applications? Different patterns suited to different situations based only on parameter changes?

    Existential question. Life on Earth. 

    At start of presentation: why? What is the point?

    Measure complexity and entropy over time.

    Measure particular outcomes and then explain how these outcomes change when the parameters are changed.

Tasks

### Expectation list of parameters

``` python
'density'       : calc_density,
'location'      : calc_average_loc,
'network_count' : calc_num_networks,
'network_size'  : calc_network_size,
'area'          : calc_area,
'entropy'       : calc_entropy}

mate_radius            40.0

neighbor_radius        50.0 
search_radius          200.0

Search > neighbor, strange extremes, same...

move_modifier          5
underpopulation_limit  1
overpopulation_limit   10
crowded_limit          3
regeneration_amount    0.
underpopulation_damage 1.
overpopulation_damage  1.
affection_prime        .09
affection_threshold    10.
kids_per_reproduction  2.
turn_rate              30
cell_life              10
cell_size              10

border_damage          100.0
max_iterations         100

random_start           true
debug                  false 
borders                none 
```

3 params
variances for each
metrics

boxplots
