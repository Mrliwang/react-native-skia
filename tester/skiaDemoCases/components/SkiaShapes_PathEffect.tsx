import React, {Component} from 'react';
import {View} from 'react-native';

import {
  Canvas,
  rect,
  Path,
  Skia,
  Group,
  Circle,
  DiscretePathEffect,
  DashPathEffect,
  Rect,
  CornerPathEffect,
  Path1DPathEffect,
  Path2DPathEffect,
  processTransform2d,
  Line2DPathEffect,
  Vector,
  sub,
  transformOrigin,
} from '@shopify/react-native-skia';

import {ScrollView} from 'react-native';
import {Tester, TestCase} from '@rnoh/testerino';
import {translate} from '../lib/Animations';

const vWidth = 466;
const vHeight = 182;
const vOrigin = {x: vWidth / 2, y: vHeight / 2};

const path = Skia.Path.MakeFromSVGString(
  // eslint-disable-next-line max-len
  'M466 91C466 141.258 361.682 182 233 182C104.318 182 0 141.258 0 91C0 40.7421 104.318 0 233 0C361.682 0 466 40.7421 466 91Z',
)!;

const Logo: React.FC<{center: Vector; origin: Vector; scale: number}> = ({
  center,
  origin,
  scale,
}) => {
  return (
    <>
      <Circle c={center} r={30} style="fill" />
      <Group>
        <Group transform={[...translate(sub(center, origin)), {scale}]}>
          <Path path={path} style="stroke" strokeWidth={15} />
        </Group>
        <Group
          transform={[
            ...translate(sub(center, origin)),
            {scale},
            ...transformOrigin(vOrigin, [{rotate: Math.PI / 3}]),
          ]}>
          <Path path={path} style="stroke" strokeWidth={15} />
        </Group>
        <Group
          transform={[
            ...translate(sub(center, origin)),
            {scale},
            ...transformOrigin(vOrigin, [{rotate: -Math.PI / 3}]),
          ]}>
          <Path path={path} style="stroke" strokeWidth={15} />
        </Group>
      </Group>
    </>
  );
};

const rect1 = rect(0, 0, vWidth, vHeight);
const SquaredLogo: React.FC<{
  center: Vector;
  origin: Vector;
  scale: number;
}> = ({center, origin, scale}) => {
  return (
    <>
      <Circle c={center} r={30} style="fill" />
      <Group color="#61DAFB" opacity={1} style="stroke" strokeWidth={15}>
        <Group transform={[...translate(sub(center, origin)), {scale}]}>
          <Rect rect={rect1} />
        </Group>
        <Group
          transform={[
            ...translate(sub(center, origin)),
            {scale},
            ...transformOrigin(vOrigin, [{rotate: Math.PI / 3}]),
          ]}>
          <Rect rect={rect1} />
        </Group>
        <Group
          transform={[
            ...translate(sub(center, origin)),
            {scale},
            ...transformOrigin(vOrigin, [{rotate: -Math.PI / 3}]),
          ]}>
          <Rect rect={rect1} />
        </Group>
      </Group>
    </>
  );
};

const width = 256;
const SIZE = width;
const scale = (SIZE - 64) / vWidth;
const origin = {x: (vWidth * scale) / 2, y: (vHeight * scale) / 2};
const center = {x: SIZE / 2, y: SIZE / 2};
const styles = {width: 256, height: width};

class DiscretePathEffectExample extends Component {
  render() {
    return (
      <View style={{padding: 10, alignItems: 'center'}}>
        <Canvas style={styles}>
          <Group color="#61DAFB" style="stroke" strokeWidth={15}>
            <DiscretePathEffect length={10} deviation={4} />
            <Logo center={center} origin={origin} scale={scale} />
          </Group>
        </Canvas>
      </View>
    );
  }
}

class DashPathEffectExample extends Component {
  render() {
    return (
      <View style={{padding: 10, alignItems: 'center'}}>
        <Canvas style={styles}>
          <Group color="#61DAFB" style="stroke" strokeWidth={15}>
            <DashPathEffect intervals={[10, 10]} />
            <Logo center={center} origin={origin} scale={scale} />
          </Group>
        </Canvas>
      </View>
    );
  }
}

class CornerPathEffectExample extends Component {
  render() {
    return (
      <View style={{padding: 10, alignItems: 'center'}}>
        <Canvas style={styles}>
          <Group color="#61DAFB" style="stroke" strokeWidth={15} opacity={0.5}>
            <Logo center={center} origin={origin} scale={scale} />
          </Group>
          <Group color="#61DAFB" style="stroke" strokeWidth={15}>
            <CornerPathEffect r={200} />
            <SquaredLogo center={center} origin={origin} scale={scale} />
          </Group>
        </Canvas>
      </View>
    );
  }
}

class Path1DPathEffectExample extends Component {
  render() {
    return (
      <View style={{padding: 10, alignItems: 'center'}}>
        <Canvas style={styles}>
          <Group color="#61DAFB" style="stroke" strokeWidth={15}>
            <Path1DPathEffect
              path="M -10 0 L 0 -10, 10 0, 0 10 Z"
              advance={20}
              phase={0}
              style="rotate"
            />
            <Logo center={center} origin={origin} scale={scale} />
          </Group>
        </Canvas>
      </View>
    );
  }
}

class Path2DPathEffectExample extends Component {
  render() {
    return (
      <View style={{padding: 10, alignItems: 'center'}}>
        <Canvas style={styles}>
          <Group color="#61DAFB" style="stroke" strokeWidth={15}>
            <Path2DPathEffect
              path="M -10 0 L 0 -10, 10 0, 0 10 Z"
              matrix={processTransform2d([{scale: 40}])}
            />
            <Logo center={center} origin={origin} scale={scale} />
          </Group>
        </Canvas>
      </View>
    );
  }
}

class Line2DPathEffectExample extends Component {
  render() {
    return (
      <View style={{padding: 10, alignItems: 'center'}}>
        <Canvas style={styles}>
          <Group color="#61DAFB" style="stroke" strokeWidth={15}>
            <Line2DPathEffect
              width={0}
              matrix={processTransform2d([{scale: 8}])}
            />
            <Logo center={center} origin={origin} scale={scale} />
          </Group>
        </Canvas>
      </View>
    );
  }
}

class ComposePathEffectExample extends Component {
  render() {
    return (
      <View style={{padding: 10, alignItems: 'center'}}>
        <Canvas style={styles}>
          <Group color="#61DAFB" style="stroke" strokeWidth={15}>
            <DashPathEffect intervals={[10, 10]}>
              <DiscretePathEffect length={10} deviation={10} />
            </DashPathEffect>
            <Logo center={center} origin={origin} scale={scale} />
          </Group>
        </Canvas>
      </View>
    );
  }
}

export default function () {
  return (
    <Tester style={{flex: 1}}>
      <ScrollView>
        <TestCase itShould="case1: Discrete Path Effect">
          <DiscretePathEffectExample />
        </TestCase>

        <TestCase itShould="case2: Dash Path Effect">
          <DashPathEffectExample />
        </TestCase>

        <TestCase itShould="case3: Corner Path Effect">
          <CornerPathEffectExample />
        </TestCase>

        <TestCase itShould="case4: Path1D Path Effect">
          <Path1DPathEffectExample />
        </TestCase>

        <TestCase itShould="case5: Path2D Path Effect">
          <Path2DPathEffectExample />
        </TestCase>

        <TestCase itShould="case6: Line2D Path Effect">
          <Line2DPathEffectExample />
        </TestCase>

        <TestCase itShould="case7: Compose Path Effect">
          <ComposePathEffectExample />
        </TestCase>
      </ScrollView>
    </Tester>
  );
}