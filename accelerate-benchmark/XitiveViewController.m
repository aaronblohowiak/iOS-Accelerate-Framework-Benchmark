//
//  XitiveViewController.m
//  accelerate-benchmark
//
//  Created by Aaron Blohowiak on 2/20/12.
//  Copyright (c) 2012 none. All rights reserved.
//

#import "XitiveViewController.h"
#import "XitiveBenchmark.h"

@implementation XitiveViewController

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Release any cached data, images, etc that aren't in use.
}

#pragma mark - View lifecycle

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    
    UIButton * btn = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    [btn setTitle:@"Benchmark!" forState:UIControlStateNormal];
    [btn addTarget:self action:@selector(benchmark:) forControlEvents:UIControlEventTouchUpInside];
    btn.frame = CGRectMake(30, 30, 200, 75);
    [self.view addSubview:btn];
    
    label = [[UILabel alloc] initWithFrame:CGRectMake(30, 110, 200, 75 )];
    [self.view addSubview:label];
}

- (void)benchmark:(id)sender{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^(void){
        NSString * path = [[NSHomeDirectory() stringByAppendingPathComponent:@"Documents/"] stringByAppendingPathComponent:@"output.csv"];
        
        NSLog(@"%@", [path debugDescription]);
        const char * filename = [path cStringUsingEncoding:NSUTF8StringEncoding];
        
        NSString * resourcePath = [[NSBundle mainBundle] resourcePath];
        
        const char * track_a = [[resourcePath stringByAppendingPathComponent:@"track_a.wav"] cStringUsingEncoding:NSUTF8StringEncoding];
        
        const char * track_b = [[resourcePath stringByAppendingPathComponent:@"track_b.wav"] cStringUsingEncoding:NSUTF8StringEncoding];

        
        write_benchmark_to_file((char *)filename, (char *)track_a, (char *)track_b, 1000);
        [self performSelectorOnMainThread:@selector(workDone:) withObject:nil waitUntilDone:NO];
    });
}

-(void)workDone:(id)sender{
    [[[UIAlertView alloc] initWithTitle:@"Work done!" message:@"Benchmark has been written to csv" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil, nil] show];
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
	[super viewWillDisappear:animated];
}

- (void)viewDidDisappear:(BOOL)animated
{
	[super viewDidDisappear:animated];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
        return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
    } else {
        return YES;
    }
}

@end
