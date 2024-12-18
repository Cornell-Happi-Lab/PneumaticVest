% Define Input Parameters
L = 10.5;  % Length of the pouch (cm)
W = 10.5;  % Width of the pouch (cm)
h_final_top = 30;  % Final constrained height for the top (mm)
h_final_bottom = 10;  % Final constrained height for the bottom (mm)
num_frames = 100;  % Number of frames for inflation or deflation

% Initialize figure
figure;
hold on;

% Total frames for inflation + deflation
total_frames = 2 * num_frames;

% Loop through frames to simulate inflation and deflation
for frame = 1:total_frames
    if frame <= num_frames
        % Inflation Phase
        h_top = (frame / num_frames) * h_final_top;  % Linear inflation for top
        h_bottom = (frame / num_frames) * h_final_bottom;  % Linear inflation for bottom
    else
        % Deflation Phase
        h_top = ((total_frames - frame) / num_frames) * h_final_top;  % Linear deflation for top
        h_bottom = ((total_frames - frame) / num_frames) * h_final_bottom;  % Linear deflation for bottom
    end

    % Define semi-major and semi-minor axes for the bottom ellipse
    a_bottom = (L / 2) - 1;  % Semi-major axis for bottom ellipse
    b_bottom = h_bottom / 20;  % Semi-minor axis for bottom ellipse

    % Define semi-major and semi-minor axes for the top ellipse
    a_top = a_bottom;  % Top ellipse shares the same semi-major axis
    b_top = h_top / 20;  % Semi-minor axis for top ellipse

    % Generate the bottom ellipse
    theta_bottom = linspace(pi, 2*pi, 100);  % Bottom half of ellipse
    x_bottom = a_bottom * cos(theta_bottom) + L / 2;
    y_bottom = b_bottom * sin(theta_bottom);  % Bottom starts at y = 0

    % Generate the top ellipse, ensuring endpoints align with the bottom
    theta_top = linspace(0, pi, 100);  % Top half of ellipse
    x_top = a_top * cos(theta_top) + L / 2;
    y_top = b_top * sin(theta_top) + y_bottom(end);  % Align top ends to bottom ends

    % Combine the top and bottom halves
    x_combined = [x_top, x_bottom];
    y_combined = [y_top, y_bottom];

    % Clear previous frame and plot the updated shape
    cla;
    plot(x_combined, y_combined, 'b-', 'LineWidth', 2);  % Asymmetric shape
    hold on;

    % Add height measurement lines
    plot([L / 2, L / 2], [0, b_top + y_bottom(end)], 'm--', 'LineWidth', 1.5); % Top height
    plot([L / 2, L / 2], [0, b_bottom], 'm--', 'LineWidth', 1.5); % Bottom height

    % Annotate the height measurements
    text(L / 2 + 1, b_top + y_bottom(end), sprintf('h_{top} = %.2f mm', h_top), ...
        'Color', 'green', 'FontSize', 10, 'HorizontalAlignment', 'left');
    text(L / 2 + 1, b_bottom, sprintf('h_{bottom} = %.2f mm', h_bottom), ...
        'Color', 'green', 'FontSize', 10, 'HorizontalAlignment', 'left');

    % Adjust axis and labels
    axis([0 L -h_final_bottom/10 h_final_top/10 + b_bottom]);  % Dynamic axis scaling
    axis equal;
    title(sprintf('Frame %d/%d: h_{top} = %.2f mm, h_{bottom} = %.2f mm', ...
        frame, total_frames, h_top, h_bottom));
    xlabel('X (cm)');
    ylabel('Y (cm)');
    grid on;

    % Refresh the plot to display the frame
    drawnow;  % Ensures the figure updates in real time
    pause(0.05);  % Adjust the pause for desired frame rate
end
