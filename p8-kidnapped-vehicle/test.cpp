    // variables
    double sigma_x2 = pow(std_landmark[0],2);
    double sigma_y2 = pow(std_landmark[1],2);

    // cycle through particles and update weights

    for (auto&& particle : particles){

        // particle error
        double w = 1.0;

        // cycle through observations (id / x / y)
        for (auto&& observation : observations){

          double obs_x = particle.x + (observation.x * cos(particle.theta) - observation.y * sin(particle.theta));
          double obs_y = particle.y + (observation.x * sin(particle.theta) + observation.y * cos(particle.theta));

          double distance = sensor_range;
          int land_idx = -1;

          for (auto&& landmark : map_landmarks.landmark_list){

            double obs_dist_to_land = dist(obs_x, obs_y,  landmark.x_f, landmark.y_f);

            if (obs_dist_to_land < distance) {
              land_idx = landmark.id_i - 1;
              distance = obs_dist_to_land;
            }
          }

          if (land_idx >= 0) {

            long double diff_x = (map_landmarks.landmark_list[land_idx].x_f - obs_x);
            long double diff_y = (map_landmarks.landmark_list[land_idx].y_f - obs_y);

            long double a =  pow( diff_x, 2) / sigma_x2;
            long double b =  pow( diff_y, 2) / sigma_y2;

            long double w_i = ( 1 / (2 * M_PI * std_landmark[0] * std_landmark[1]) ) * exp( (-1/2.) * (a + b) );

            w *= w_i;
      }

    }

    particle.weight = w;

    }
